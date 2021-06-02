#ifndef UARTINTERFACE_H
#define UARTINTERFACE_H

#include "DataStore.h"
#include "SimObject.h"
#include <stdint.h>
#include <map>
#include <vector>
#include <functional>
#include <termios.h>
#include <unistd.h>

/**
 * Parameters:
 *      Calling object
 *      Byte received
 */
typedef void (*UART_Callback)(void*, uint8_t);

class UARTInterface : public SimObject
{
private:
    struct UART_Callback_Store {
        UART_Callback method;
        void* caller;
    };
    /**
     * Callback methods
     */
    std::vector<UART_Callback_Store> m_callbacks;

    /**
     * Entry point for UART Rx monitoring thread
     */
    static void* UARTThread(void* UARTInterface);

    /**
     * Used to only print Tx errors once
     */
    bool m_errorPrinted;

    /**
     * File descriptor
     */
    int m_fd;

    struct termios m_uartTermios;

public:
    UARTInterface(DataStore& dataStore);

    virtual void init();

    virtual void step(unsigned long dt);

    void addCallback(UART_Callback callbackMethod, void* obj);

    /**
     * @brief Write a UART frame to the bus
     * @param len Length of data
     * @param data Data to send
     */
    void send(const uint8_t* data, const uint8_t len);
};

#endif