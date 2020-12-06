#include "MCP4912.h"

#include <iostream>
#include <cstdint>

// Setup control bit constants
const uint8_t MCP4912::CHANNEL_A = 0;
const uint8_t MCP4912::CHANNEL_B = 1;
const uint8_t MCP4912::BUFFERED = 1;
const uint8_t MCP4912::UNBUFFERED = 0;
const uint8_t MCP4912::GAIN_1 = 1;
const uint8_t MCP4912::GAIN_2 = 0;
const uint8_t MCP4912::ACTIVE = 1;
const uint8_t MCP4912::SHDN = 0;

MCP4912::MCP4912(
    DataStore& dataStore, 
    std::shared_ptr<GPIOInterface> gpioInterface,
    std::shared_ptr<SPIInterface> spiInterface)
    : SimObject(dataStore, "MCP4912", 1),  // 1Hz
      m_gpio(gpioInterface),
      m_spi(spiInterface)
{
    // set chip-select pin as GPIO output
}

void MCP4912::init()
{
    std::cout << "[" << getName() << "] init" << std::endl;
    // No variables to be initialized
}

void MCP4912::step(unsigned long)
{
    // Nothing to be done here for this sim device
}

void MCP4912::initChannel(const OutputChannel& channel)
{
    // Allow output on pin and set to 0
    m_gpio->initPin(channel.csPin, GPIOInterface::GPIO_OUT);
    m_gpio->writePin(channel.csPin, 1); // Mode 0 => !CS=1 for not selected

    // set LDAC
    m_gpio->initPin(channel.ldacPin, GPIOInterface::GPIO_OUT);
    m_gpio->writePin(channel.ldacPin, 1); // Update Vout on rising CS

    // set SHDN
    m_gpio->initPin(channel.shdnPin, GPIOInterface::GPIO_OUT);
    m_gpio->writePin(channel.shdnPin, 1); // Allow output
}

void MCP4912::write(
    uint16_t value, 
    const OutputChannel& channel,
    const uint8_t bufferControl,
    const uint8_t outputGain,
    const uint8_t shutdownCtrl)
{
    uint8_t tx[2] = {0};
    uint8_t rx[2] = {0};  // Unused, but a buffer is needed

    // Encode data
    tx[0] |= (channel.channel & 0x1) << 7;
    tx[0] |= (bufferControl & 0x1) << 6;
    tx[0] |= (outputGain & 0x1) << 6;
    tx[0] |= (shutdownCtrl & 0x1) << 6;

    tx[0] |= (value & 0x3FF) >> 6;
    tx[1] |= (value & 0x3FF) << 2;

    // Perform data transfer
    m_spi->transfer(rx, tx, 2, channel.csPin);
}