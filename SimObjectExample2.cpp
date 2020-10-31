#include "SimObjectExample2.h"

#include <iostream>
#include <any>

SimObjectExample2::SimObjectExample2(DataStore& dataStore)
    : SimObject(dataStore, "Example2", 1)  // 1Hz
{

}

void SimObjectExample2::init()
{
    std::cout << "[" << getName() << "] init" << std::endl;

    unsigned long counter = 0;
    getDataStore().put("counter", counter);
}

void SimObjectExample2::step(unsigned long dt)
{
    DataStore& ds = getDataStore();
    unsigned long counter = ds.get<unsigned long>("counter");

    double delay = 1e-9*dt;
    double hz = 1/delay;
    std::cout << "[" << getName() << "] tick " << delay << "s (" << hz << "Hz)\t";
    std::cout << "counter: " << counter << std::endl;


    counter = 0;
    ds.put("counter", counter);

}