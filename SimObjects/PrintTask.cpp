#include "PrintTask.h"

#include <iostream>
#include <any>

PrintTask::PrintTask(DataStore& dataStore)
    : SimObject(dataStore, "PrintTask", 1)  // 1Hz
{

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

}