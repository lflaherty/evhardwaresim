#include "CANInterface.h"

#include <iostream>
#include <any>
#include <cstring>

#include <pthread.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include "linux/can.h"
#include "linux/can/raw.h"

CANInterface::CANInterface(DataStore& dataStore)
    : SimObject(dataStore, "CAN Interface", 1),  // 1Hz
      m_errorPrinted(false)
{
	// Set up CAN driver
	struct sockaddr_can addr;
	struct ifreq ifr;

	const char *ifname = "can0";

	if((m_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("[CAN Interface] Error while opening socket");
	}

	strcpy(ifr.ifr_name, ifname);
	ioctl(m_socket, SIOCGIFINDEX, &ifr);
	
	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	std::cout << "[" << getName() << "] ";
    std::cout << ifname << " at index " << ifr.ifr_ifindex << std::endl;

	if(bind(m_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("[CAN Interface] Error in socket bind");
	}
	
	// Set up CAN socket threads
    struct sched_param param;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    param.sched_priority = 1;
    pthread_attr_setschedparam(&attr, &param);
	pthread_t threadTask_id;
    pthread_create(&threadTask_id, &attr, canThread, this);

}

void CANInterface::init()
{
    std::cout << "[" << getName() << "] init" << std::endl;
    // No variables to be initialized
}

void CANInterface::step(unsigned long)
{
    // Nothing to be done here for this sim device
}

void CANInterface::addCallback(CAN_Callback callbackMethod, void* obj)
{
    CAN_Callback_Store store;
    store.method = callbackMethod;
    store.caller = obj;
    m_callbacks.push_back(store);
}

void CANInterface::send(const uint16_t canId, uint8_t len, const uint8_t* data)
{
    struct can_frame frame;
    frame.can_id = canId;
    frame.can_dlc = len;

    // copy data to can_frame
    memcpy(frame.data, data, len);

    int retval = write(m_socket, &frame, sizeof(frame));
    if (retval < 0 && !m_errorPrinted)
    {
        perror("[CAN Interface] Error sending CAN message");
        m_errorPrinted = true;
    }

}

void* CANInterface::canThread(void* canInterface)
{
    CANInterface* can = (CANInterface*)canInterface;

    bool printedError = false;
    
    while(true) {
        // struct timeval timeout = {1, 0};  // timeout for 1s
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(can->m_socket, &readSet);

        if (FD_ISSET(can->m_socket, &readSet)) {
            // read CAN frames
            struct can_frame receiveMsg;      
            int recvbytes = read(can->m_socket, &receiveMsg, sizeof(receiveMsg));

            if (recvbytes == -1) {
                // only print error once
                if (!printedError) {
                    std::cout << "[" << can->getName() << "] Error reading CAN message";
                    printedError = true;
                }
            } else {
                // Received data

                // Get the appropriate callbacks
                uint32_t canId = receiveMsg.can_id;
                for (CAN_Callback_Store callback : can->m_callbacks) {
                    // call them all
                    callback.method(callback.caller, canId, receiveMsg.data, receiveMsg.can_dlc);
                }
            }
        }
    }

    return NULL;
}