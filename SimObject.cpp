#include "SimObject.h"

#include <iostream>
#include <string>
#include "time.h"

SimObject::SimObject(DataStore& dataStore, std::string name, unsigned int tickRate)
    : m_dataStore(dataStore), m_name(name)
{
    // initialize initial time
    m_delay = 1e9 / tickRate;
    clock_gettime(CLOCK_MONOTONIC, &m_ts);
}

SimObject::~SimObject()
{
}

std::string SimObject::getName() 
{ 
    return m_name; 
}

DataStore& SimObject::getDataStore() 
{ 
    return m_dataStore; 
}

void* SimObject::run(void* obj)
{
    SimObject* simObject = (SimObject*)obj;
    std::cout << "[" << simObject->m_name << "] starting sim" << std::endl;

    while (true)
    {
        unsigned long measuredDt = simObject->sleep_until_next();
        simObject->step(measuredDt);
    }

    return NULL;
}

unsigned SimObject::sleep_until_next()
{
    m_ts.tv_nsec += m_delay;
    if (m_ts.tv_nsec >= 1e9)  // check if delay increased ts to the next second
    {
        m_ts.tv_nsec -= 1e9;
        m_ts.tv_sec++;
    }

    struct timespec tstart={0,0}, tend={0,0};
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    
    // perform sleep
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &m_ts, NULL);

    clock_gettime(CLOCK_MONOTONIC, &tend);

    unsigned long measuredDelay = 
        (1e9*tend.tv_sec + tend.tv_nsec) - 
        (1e9*tstart.tv_sec + tstart.tv_nsec);
    return measuredDelay;
}