#ifndef SIMOBJECTEXAMPLE1_H
#define SIMOBJECTEXAMPLE1_H

#include "DataStore.h"
#include "SimObject.h"

class SimObjectExample1 : public SimObject
{
public:
    SimObjectExample1(DataStore& dataStore);

    virtual void init();

    virtual void step(unsigned long dt);
};

#endif