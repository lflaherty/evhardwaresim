#ifndef CANINTERFACE_H
#define CANINTERFACE_H

#include "DataStore.h"
#include "SimObject.h"
#include <stdint.h>
#include <map>
#include <vector>
#include <functional>

/**
 * Parameters:
 *      Calling object
 *      Msg ID
 *      Data payload
 *      Length of data payload (up to 8)
 */
typedef void (*CAN_Callback)(void*, uint32_t, uint8_t[8], size_t);

class CANInterface : public SimObject
{
private:
    struct CAN_Callback_Store {
        CAN_Callback method;
        void* caller;
    };
    /**
     * Callback methods
     */
    std::vector<CAN_Callback_Store> m_callbacks;

    /**
     * Socket for CAN bus network
     */
    int m_socket;

    /**
     * Entry point for CAN Rx monitoring thread
     */
    static void* canThread(void* canInterface);

    /**
     * Used to only print Tx errors once
     */
    bool m_errorPrinted;

public:
    CANInterface(DataStore& dataStore);

    virtual void init();

    virtual void step(unsigned long dt);

    void addCallback(CAN_Callback callbackMethod, void* obj);

    /**
     * @brief Write a CAN frame to the bus
     * @param canId CAN frame ID
     * @param len Length of data
     * @param data Data to send
     */
    void send(const uint16_t canId, uint8_t len, const uint8_t* data);
};

#endif