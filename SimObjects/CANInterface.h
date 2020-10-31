#ifndef CANINTERFACE_H
#define CANINTERFACE_H

#include "DataStore.h"
#include "SimObject.h"
#include <stdint.h>
#include <map>
#include <vector>

typedef void (*CAN_Callback)(uint32_t, uint8_t*, size_t);

class CANInterface : public SimObject
{
private:
    /**
     * Callback methods
     * Maps Msg ID -> methods to call
     */
    std::map<uint32_t, std::vector<CAN_Callback>> m_callbacks;

    int m_socket;

    static void* canThread(void* canInterface);

    std::vector<CAN_Callback> getCallbacks(uint32_t canMsgId);

public:
    CANInterface(DataStore& dataStore);

    virtual void init();

    virtual void step(unsigned long dt);

    void addCallback(uint32_t msgId, CAN_Callback callbackMethod);
};

#endif