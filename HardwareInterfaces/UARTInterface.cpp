#include "UARTInterface.h"

#include <iostream>
#include <any>
#include <cstring>

#include <pthread.h>
#include <stddef.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

UARTInterface::UARTInterface(DataStore& dataStore)
    : SimObject(dataStore, "UART Interface", 1),  // 1Hz
      m_errorPrinted(false)
{
	// Set up UART driver
    char ttyFile[] = "/dev/ttyS4";

    m_fd = open(ttyFile, O_RDWR | O_NOCTTY);
    if (m_fd < 0) {
		perror("[UARTInterface] Error while opening serial file");
    }

    bzero(&m_uartTermios,sizeof(m_uartTermios));


    // Choose the control/input/output/local modes for UART operation
    // http://man7.org/linux/man-pages/man3/termios.3.html
    int baud = B9600;
    m_uartTermios.c_cflag = baud | CS8 | CLOCAL | CREAD;
    m_uartTermios.c_iflag = IGNPAR | ICRNL | IGNCR;
    m_uartTermios.c_oflag = 0;
    m_uartTermios.c_lflag = 0;

    m_uartTermios.c_cc[VTIME] = 0;
    m_uartTermios.c_cc[VMIN]  = 0;

    // clean line and set attributes
    tcflush(m_fd, TCIFLUSH);
    tcsetattr(m_fd, TCSANOW, &m_uartTermios);
	
	// Set up UART socket threads
    struct sched_param param;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    param.sched_priority = 1;
    pthread_attr_setschedparam(&attr, &param);
	pthread_t threadTask_id;
    pthread_create(&threadTask_id, &attr, UARTThread, this);

    std::cout << "[" << getName() << "] device setup complete" << std::endl;
}

void UARTInterface::init()
{
    std::cout << "[" << getName() << "] init" << std::endl;
    // No variables to be initialized
}

void UARTInterface::step(unsigned long)
{
    // Nothing to be done here for this sim device
}

void UARTInterface::addCallback(UART_Callback callbackMethod, void* obj)
{
    UART_Callback_Store store;
    store.method = callbackMethod;
    store.caller = obj;
    m_callbacks.push_back(store);
}

void UARTInterface::send(const uint8_t* data, const uint8_t len)
{
    int retval = write(m_fd, data, len);
    
    if (retval < 0 && !m_errorPrinted)
    {
        perror("[UARTInterface] Error sending UART message");
        m_errorPrinted = true;
    }
}

void* UARTInterface::UARTThread(void* uartInterface)
{
    UARTInterface* uart = static_cast<UARTInterface*>(uartInterface);

    bool printedError = false;
    
    while(true) {
        uint8_t receivedByte = 0;
        int recvbytes = read(uart->m_fd, &receivedByte, sizeof(receivedByte));

        if (recvbytes == -1) {
            // only print error once
            if (!printedError) {
                std::cout << "[" << uart->getName() << "] Error reading UART byte";
                printedError = true;
            }
        } else if (recvbytes > 0) {
            // Received data

            // Get the appropriate callbacks
            for (UART_Callback_Store callback : uart->m_callbacks) {
                callback.method(callback.caller, receivedByte);
            }
        }
    }

    return NULL;
}