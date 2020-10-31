#ifndef SIM_H
#define SIM_H

#include "DataStore.h"

#include "time.h"
#include <string>

class SimObject
{
private:
    /**
     * DataStore object for shared data storage
     */
    DataStore& m_dataStore;

    /**
     * Name of current sim obj
     */
    std::string m_name; 

    /**
     * Period in ms
     */
    unsigned long m_delay;

    /**
     * Latest timespec
     */
    struct timespec m_ts;

    /**
     * Sleep current thread for next delay
     */
    unsigned sleep_until_next();

protected:   

public:
    SimObject(DataStore& dataStore, std::string name, unsigned int tickRate);
    virtual ~SimObject();

    std::string getName();

    DataStore& getDataStore();

    /**
     * Start this object's sim processing
     */
    static void* run(void* obj);

    /**
     * Initialize object's simulation data
     */
    virtual void init() = 0;

    /**
     * Run step of simulation
     * dt measured delta in ns
     */
    virtual void step(unsigned long dt) = 0;

};

#endif