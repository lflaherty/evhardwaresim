#ifndef PrintTask_H
#define PrintTask_H

#include "DataStore.h"
#include "SimObject.h"

#include "CANInterface.h"

#include <memory>

class PrintTask : public SimObject
{
private:
    static void canCallback(void* obj, uint32_t msgId, uint8_t* data, size_t);
public:
    PrintTask(DataStore& dataStore, std::shared_ptr<CANInterface> canInterface);

    virtual void init();

    virtual void step(unsigned long dt);
};

#endif