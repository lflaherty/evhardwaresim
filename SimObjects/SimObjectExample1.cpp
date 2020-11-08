#include "SimObjectExample1.h"

#include "GPIOInterface.h"
#include <iostream>
#include <any>
#include <cstdint>

// Declare channels
static const MCP4912::OutputChannel ChannelA {
    .csPin = &GPIOInterface::P9_23,
    .channel = MCP4912::CHANNEL_A
};
static const MCP4912::OutputChannel ChannelB {
    .csPin = &GPIOInterface::P9_23,
    .channel = MCP4912::CHANNEL_B
};
static const MCP4912::OutputChannel ChannelC {
    .csPin = &GPIOInterface::P9_25,
    .channel = MCP4912::CHANNEL_A
};
static const MCP4912::OutputChannel ChannelD {
    .csPin = &GPIOInterface::P9_25,
    .channel = MCP4912::CHANNEL_B
};


SimObjectExample1::SimObjectExample1(
    DataStore& dataStore, 
    std::shared_ptr<MCP4912> dac)
    : SimObject(dataStore, "Example1", 100),  // 100Hz
      m_mcp4912(dac)
{
    // initialize device and start with known DAC values
    m_mcp4912->initChannel(ChannelA);
    m_mcp4912->initChannel(ChannelB);
    m_mcp4912->initChannel(ChannelC);
    m_mcp4912->initChannel(ChannelD);

    m_mcp4912->write(0, ChannelA, MCP4912::BUFFERED, MCP4912::GAIN_1, MCP4912::ACTIVE);
    m_mcp4912->write(0, ChannelB, MCP4912::BUFFERED, MCP4912::GAIN_1, MCP4912::ACTIVE);
    m_mcp4912->write(0, ChannelC, MCP4912::BUFFERED, MCP4912::GAIN_1, MCP4912::ACTIVE);
    m_mcp4912->write(0, ChannelD, MCP4912::BUFFERED, MCP4912::GAIN_1, MCP4912::ACTIVE);
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

    // also write to one of the DACs
    uint16_t dacOutput = counter % 4096;
    m_mcp4912->write(dacOutput, ChannelA, MCP4912::BUFFERED, MCP4912::GAIN_1, MCP4912::ACTIVE);

}