#ifndef PrintTask_H
#define PrintTask_H

#include "DataStore.h"
#include "SimObject.h"

#include "CANInterface.h"

#include <memory>
#include <vector>

class PrintTask : public SimObject
{
private:
    static void canCallback(void* obj, uint32_t msgId, uint8_t* data, size_t);

    /**
     * Used to access DAC
     */
    std::shared_ptr<CANInterface> m_can;

    struct CANFrame {
        uint32_t msgId;
        uint8_t data[8];
        size_t len;
    };

    /**
     * Stores history of received CAN messages
     */
    std::vector<CANFrame> m_receivedMsgs;

public:
    PrintTask(DataStore& dataStore, std::shared_ptr<CANInterface> canInterface);

    virtual void init();

    virtual void step(unsigned long dt);
};

#endif