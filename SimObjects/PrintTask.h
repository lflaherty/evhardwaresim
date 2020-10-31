#ifndef PrintTask_H
#define PrintTask_H

#include "DataStore.h"
#include "SimObject.h"

class PrintTask : public SimObject
{
public:
    PrintTask(DataStore& dataStore);

    virtual void init();

    virtual void step(unsigned long dt);
};

#endif