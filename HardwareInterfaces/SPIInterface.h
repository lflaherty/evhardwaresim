#ifndef SPIINTERFACE_H
#define SPIINTERFACE_H

#include "DataStore.h"
#include "SimObject.h"
#include "GPIOInterface.h"
#include <cstdint>
#include <memory>
#include <string>

class SPIInterface : public SimObject
{
private:
    /**
     * Used to toggle the chip select GPIO pins
     */
    std::shared_ptr<GPIOInterface> m_gpio;

    /**
     * Used to only print running errors once
     */
    bool m_errorPrinted;

    /**
     * SPI file descriptor
     */
    int m_fd;

public:
    struct SPIConfig {
        std::string device;       // path to device
        uint8_t mode;       // SPI mode. NOTE: currently only supports mode 0
        uint8_t bits;       // bits per word
        uint8_t firstBit;   // 0=MSB first, 1=LSB first
        uint32_t speed;     // Clock rate
    };
    SPIInterface(DataStore& dataStore, std::shared_ptr<GPIOInterface> gpioInterface, const SPIConfig& config);

    virtual void init();

    virtual void step(unsigned long dt);

    /**
     * @brief Perform a synchronous SPI transfer
     * @param rx Receive buffer. Will be populated with received data.
     * @param tx Transmit buffer. Data here will be sent.
     * @param len Length of data transfer
     * @param cs Chip Select pin
     */
    void transfer(uint8_t* rx, uint8_t* tx, uint8_t len, const GPIOInterface::Pin* cs);
};

#endif