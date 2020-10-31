#include "SimObjectExample1.h"

#include <iostream>
#include <any>

SimObjectExample1::SimObjectExample1(DataStore& dataStore)
    : SimObject(dataStore, "Example1", 100)  // 100Hz
{

}

void SimObjectExample1::init()
{
    std::cout << "[" << getName() << "] init" << std::endl;

    unsigned long counter = 0;
    getDataStore().put("counter", counter);
}

void SimObjectExample1::step(unsigned long)
{
    DataStore& ds = getDataStore();

    // increase counter in datastore
    unsigned long counter = ds.get<unsigned long>("counter");
    ++counter;
    ds.put("counter", counter);

}