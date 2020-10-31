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

    int m_socket;

    static void* canThread(void* canInterface);

public:
    CANInterface(DataStore& dataStore);

    virtual void init();

    virtual void step(unsigned long dt);

    void addCallback(CAN_Callback callbackMethod, void* obj);
};

#endif