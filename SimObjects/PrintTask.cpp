#include "PrintTask.h"

#include <iostream>
#include <iomanip>
#include <any>
#include <cstring>

PrintTask::PrintTask(DataStore& dataStore, std::shared_ptr<CANInterface> canInterface)
    : SimObject(dataStore, "PrintTask", 1),  // 1Hz
      m_can(canInterface),
      m_clearOnPeriodicPrint(false)
{
    std::cout << "[PrintTask] Initializing" << std::endl;

    // Register a callback
    canInterface->addCallback(canCallback, (void*)this);

    std::cout << "[PrintTask] Initialized" << std::endl;
}

void PrintTask::init()
{
    std::cout << "[" << getName() << "] init" << std::endl;

    unsigned long tick_counter = 0;
    getDataStore().put("tick_counter", tick_counter);
    getDataStore().put("adc0", (uint16_t)0);
    getDataStore().put("adc1", (uint16_t)0);
    getDataStore().put("adc2", (uint16_t)0);
    getDataStore().put("adc3", (uint16_t)0);
    getDataStore().put("adc4", (uint16_t)0);
}

void PrintTask::step(unsigned long dt)
{
    using namespace std;

    DataStore& ds = getDataStore();
    unsigned long tick_counter = ds.get<unsigned long>("tick_counter");

    double delay = 1e-9*dt;
    double hz = 1/delay;

    // Print output
    if (m_clearOnPeriodicPrint) {
        // write(1,"\E[H\E[2J",7);
        cout << "\E[H\E[2J";
        cout.flush();
    }
    cout << "[" << getName() << "] status output - " << delay << "s (" << hz << "Hz)\t" << endl;

    cout << "\tVariables:" << endl;
    cout << "\t\ttick_counter\t" << tick_counter << endl;
    cout << "\t\tadc0\t" << ds.get<uint16_t>("adc0") << endl;
    cout << "\t\tadc1\t" << ds.get<uint16_t>("adc1") << endl;
    cout << "\t\tadc2\t" << ds.get<uint16_t>("adc2") << endl;
    cout << "\t\tadc3\t" << ds.get<uint16_t>("adc3") << endl;
    cout << "\t\tadc4\t" << ds.get<uint16_t>("adc4") << endl;

    cout << "\tReceived CAN messages:" << endl;

    std::vector<CANFrame>::iterator it;
    for (it = m_receivedMsgs.begin(); it != m_receivedMsgs.end(); ++it) {
        cout << "\t\t";
        cout << hex << uppercase;
        cout << it->msgId << "\t";

        cout << dec;
        cout << "[" << it->len << "]" << " ";

        cout << hex << uppercase;
        for (size_t i = 0; i < it->len; ++i) {
            cout << " ";
            cout << setfill('0') << setw(2) << (unsigned int)it->data[i];
        }

        cout << endl;
    }
    cout << dec;
    cout << endl;

    m_receivedMsgs.clear();

    // reset the tick counter
    ds.put("tick_counter", (unsigned long)0);
}

void PrintTask::canCallback(void* obj, uint32_t msgId, uint8_t data[8], size_t len)
{
    CANFrame frame;
    frame.msgId = msgId;
    frame.len = len;

    std::memcpy(frame.data, data, len);

    PrintTask* printTask = (PrintTask*)obj;
    printTask->m_receivedMsgs.push_back(frame);
}

void PrintTask::setClearOnPeriodicPrint(const bool clear)
{
    m_clearOnPeriodicPrint = clear;
}