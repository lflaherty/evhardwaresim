#ifndef SIMOBJECTEXAMPLE1_H
#define SIMOBJECTEXAMPLE1_H

#include "DataStore.h"
#include "SimObject.h"
#include "MCP4912.h"

#include <memory>

class SimObjectExample1 : public SimObject
{
private:
    /**
     * Used to access DAC
     */
    std::shared_ptr<MCP4912> m_mcp4912;

public:
    SimObjectExample1(DataStore& dataStore, std::shared_ptr<MCP4912> dac);

    virtual void init();

    virtual void step(unsigned long dt);
};

#endif