#include "SimObjectExample1.h"

#include "GPIOInterface.h"
#include <iostream>
#include <any>
#include <cstdint>

// Declare channels
static const MCP4912::OutputChannel ChannelA {
    .csPin = &GPIOInterface::P9_23,
    .shdnPin = &GPIOInterface::P9_30,
    .ldacPin = &GPIOInterface::P9_29,
    .channel = MCP4912::CHANNEL_A
};
static const MCP4912::OutputChannel ChannelB {
    .csPin = &GPIOInterface::P9_23,
    .shdnPin = &GPIOInterface::P9_30,
    .ldacPin = &GPIOInterface::P9_29,
    .channel = MCP4912::CHANNEL_B
};
static const MCP4912::OutputChannel ChannelC {
    .csPin = &GPIOInterface::P9_25,
    .shdnPin = &GPIOInterface::P9_27,
    .ldacPin = &GPIOInterface::P9_28,
    .channel = MCP4912::CHANNEL_A
};
static const MCP4912::OutputChannel ChannelD {
    .csPin = &GPIOInterface::P9_25,
    .shdnPin = &GPIOInterface::P9_27,
    .ldacPin = &GPIOInterface::P9_28,
    .channel = MCP4912::CHANNEL_B
};


SimObjectExample1::SimObjectExample1(
    DataStore& dataStore, 
    std::shared_ptr<MCP4912> dac,
    std::shared_ptr<CANInterface> can)
    : SimObject(dataStore, "Example1", 100),  // 100Hz
      m_mcp4912(dac),
      m_can(can),
      m_counter_seconds(0)
{
    std::cout << "[SimObjectExample1] Initializing" << std::endl;

    // Register a callback
    can->addCallback(canCallback, (void*)this);

    // initialize device and start with known DAC values
    m_mcp4912->initChannel(ChannelA);
    m_mcp4912->initChannel(ChannelB);
    m_mcp4912->initChannel(ChannelC);
    m_mcp4912->initChannel(ChannelD);

    m_mcp4912->write(0, ChannelA, MCP4912::BUFFERED, MCP4912::GAIN_1, MCP4912::ACTIVE);
    m_mcp4912->write(500, ChannelB, MCP4912::BUFFERED, MCP4912::GAIN_1, MCP4912::ACTIVE);
    m_mcp4912->write(700, ChannelC, MCP4912::BUFFERED, MCP4912::GAIN_1, MCP4912::ACTIVE);
    m_mcp4912->write(0, ChannelD, MCP4912::BUFFERED, MCP4912::GAIN_1, MCP4912::ACTIVE);
    std::cout << "[SimObjectExample1] Initialized" << std::endl;
}

void SimObjectExample1::init()
{
    std::cout << "[" << getName() << "] init" << std::endl;

    getDataStore().put("tick_counter", (unsigned long)0);
    getDataStore().put("adc0", (uint16_t)0);
    getDataStore().put("adc1", (uint16_t)0);
    getDataStore().put("adc2", (uint16_t)0);
    getDataStore().put("adc3", (uint16_t)0);
    getDataStore().put("adc4", (uint16_t)0);

    getDataStore().put("dac0", (uint16_t)0);
    getDataStore().put("dac1", (uint16_t)0);
    getDataStore().put("dac2", (uint16_t)0);
    getDataStore().put("dac3", (uint16_t)0);
}

void SimObjectExample1::step(unsigned long)
{
    DataStore& ds = getDataStore();

    // increase tick_counter in datastore to inidicate how many ticks this has run
    unsigned long tick_counter = ds.get<unsigned long>("tick_counter");
    ++tick_counter;
    ds.put("tick_counter", tick_counter);
    
    // also send count on CAN bus
    if (tick_counter == 1) {
        uint8_t canData[8] = {0};
        canData[0] = (tick_counter >> 24) & 0xFF;
        canData[1] = (tick_counter >> 16) & 0xFF;
        canData[2] = (tick_counter >> 8) & 0xFF;
        canData[3] = (tick_counter) & 0xFF;
        m_can->send(0x3A1, 8, canData);

        m_counter_seconds++;
    }

    // also write to one of the DACs
    // uint16_t dacOutput = tick_counter % 1024;

    // DAC outputs are from 0-1023
    uint16_t dacOutput0 = 512;
    uint16_t dacOutput1 = 0;
    uint16_t dacOutput2 = 0;
    uint16_t dacOutput3 = (m_counter_seconds * 4) % 1023;  // out output DACs are 10 bits, but the STM32 DACs are 12bit
    
    // Update data storage so PrintTask can output
    getDataStore().put("dac0", dacOutput0);
    getDataStore().put("dac1", dacOutput1);
    getDataStore().put("dac2", dacOutput2);
    getDataStore().put("dac3", dacOutput3);

    m_mcp4912->write(dacOutput0, ChannelA, MCP4912::BUFFERED, MCP4912::GAIN_1, MCP4912::ACTIVE);
    m_mcp4912->write(dacOutput1, ChannelB, MCP4912::BUFFERED, MCP4912::GAIN_1, MCP4912::ACTIVE);
    m_mcp4912->write(dacOutput2, ChannelC, MCP4912::BUFFERED, MCP4912::GAIN_1, MCP4912::ACTIVE);
    m_mcp4912->write(dacOutput3, ChannelD, MCP4912::BUFFERED, MCP4912::GAIN_1, MCP4912::ACTIVE);

}

void SimObjectExample1::canCallback(void* obj, uint32_t msgId, uint8_t data[8], size_t len)
{
    SimObjectExample1* simObj = static_cast<SimObjectExample1*>(obj);

    switch (msgId) {
        case 0x100: {
            uint16_t adc0 = (data[1] << 8) | data[0];
            uint16_t adc1 = (data[3] << 8) | data[2];
            uint16_t adc2 = (data[5] << 8) | data[4];
            uint16_t adc3 = (data[7] << 8) | data[6];

            simObj->getDataStore().put("adc0", adc0);
            simObj->getDataStore().put("adc1", adc1);
            simObj->getDataStore().put("adc2", adc2);
            simObj->getDataStore().put("adc3", adc3);
            break;
        }

        case 0x101: {
            uint16_t adc4 = (data[1] << 8) | data[0];
            simObj->getDataStore().put("adc4", adc4);
            break;
        }
    }
}