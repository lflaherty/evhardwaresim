#include "PrintTask.h"

#include <iostream>
#include <any>

PrintTask::PrintTask(DataStore& dataStore, std::shared_ptr<CANInterface> canInterface)
    : SimObject(dataStore, "PrintTask", 1),  // 1Hz
      m_can(canInterface)
{
    // Register a callback
    canInterface->addCallback(canCallback, (void*)this);
}

void PrintTask::init()
{
    std::cout << "[" << getName() << "] init" << std::endl;

    unsigned long counter = 0;
    getDataStore().put("counter", counter);
}

void PrintTask::step(unsigned long dt)
{
    using namespace std;

    DataStore& ds = getDataStore();
    unsigned long counter = ds.get<unsigned long>("counter");

    double delay = 1e-9*dt;
    double hz = 1/delay;

    cout << "[" << getName() << "] status output - " << delay << "s (" << hz << "Hz)\t" << endl;;
    cout << "\tcounter\t" << counter << endl;
    cout << endl;


    counter = 0;
    ds.put("counter", counter);

    // also send count on CAN bus
    uint8_t canData[8] = {0};
    canData[0] = (counter >> 24) & 0xFF;
    canData[1] = (counter >> 16) & 0xFF;
    canData[2] = (counter >> 8) & 0xFF;
    canData[3] = (counter) & 0xFF;
    m_can->send(0x3A1, 8, canData);
}

void PrintTask::canCallback(void* obj, uint32_t msgId, uint8_t data[8], size_t len)
{
    using namespace std;

    cout << hex;
    cout << msgId << "\t";

    cout << dec;
    cout << "[" << len << "] ";

    cout << hex;
    for (size_t i = 0; i < len; ++i) {
        cout << " " << (unsigned int)data[i];
    }
    cout << endl;

    cout << dec;
}