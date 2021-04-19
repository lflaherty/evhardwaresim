#ifndef SIMOBJECTEXAMPLE1_H
#define SIMOBJECTEXAMPLE1_H

#include "DataStore.h"
#include "SimObject.h"
#include "MCP4912.h"
#include "CANInterface.h"

#include <memory>
#include <stdint.h>

class SimObjectExample1 : public SimObject
{
private:
    static void canCallback(void* obj, uint32_t msgId, uint8_t* data, size_t);

    /**
     * Used to access DAC
     */
    std::shared_ptr<MCP4912> m_mcp4912;

    /**
     * CAN bus access
     */
    std::shared_ptr<CANInterface> m_can;

    uint32_t m_counter_seconds;

public:
    SimObjectExample1(
        DataStore& dataStore, 
        std::shared_ptr<MCP4912> dac,
        std::shared_ptr<CANInterface> can);

    virtual void init();

    virtual void step(unsigned long dt);
};

#endif