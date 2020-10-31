#ifndef SIMOBJECTEXAMPLE2_H
#define SIMOBJECTEXAMPLE2_H

#include "DataStore.h"
#include "SimObject.h"

class SimObjectExample2 : public SimObject
{
public:
    SimObjectExample2(DataStore& dataStore);

    virtual void init();

    virtual void step(unsigned long dt);
};

#endif