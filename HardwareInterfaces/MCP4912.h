#ifndef MCP4912_H
#define MCP4912_H

#include "DataStore.h"
#include "SimObject.h"
#include "GPIOInterface.h"
#include "SPIInterface.h"
#include <memory>
#include <cstdint>

class MCP4912 : public SimObject
{
private:
    /**
     * Used to toggle the chip select GPIO pins
     */
    std::shared_ptr<GPIOInterface> m_gpio;

    /**
     * Used to interface to the MCP4912
     */
    std::shared_ptr<SPIInterface> m_spi;


public:
    MCP4912(DataStore& dataStore, 
            std::shared_ptr<GPIOInterface> gpioInterface,
            std::shared_ptr<SPIInterface> spiInterface);

    virtual void init();

    virtual void step(unsigned long dt);

    // Device control constants
    static const uint8_t CHANNEL_A;  // Use Channel A
    static const uint8_t CHANNEL_B;  // Use Channel B
    static const uint8_t BUFFERED;   // VREF input buffered
    static const uint8_t UNBUFFERED; // VREF input unbuffered
    static const uint8_t GAIN_1;     // output = 1 * ( VREF*D/1024 )
    static const uint8_t GAIN_2;     // output = 2 * ( VREF*D/1024 )
    static const uint8_t ACTIVE;     // Active mode operation. Vout available.
    static const uint8_t SHDN;       // Shutdown selected channel.

    /**
     * Definition of MCP4912 device to access
     */
    struct OutputChannel {
        const GPIOInterface::Pin* csPin;  // CS pin applied to MCP4912 device
        const GPIOInterface::Pin* shdnPin;  // !SHDN pin
        const GPIOInterface::Pin* ldacPin;  // !LDAC pin
        const uint8_t channel;   // 0 for 
    };

    /**
     * @brief Initialize CS pin for the given channel's device
     * @param channel Channel to initialize device for.
     */
    void initChannel(const OutputChannel& channel);

    /**
     * @brief Updates the write command register on the MCP4912.
     * @param value Value to write. Max 1024. Vout = GAIN * VREF*D/1024 
     * @param channel Device and channel to write to
     * @param bufferControl Enable or diable buffer
     * @param outputGain Set output gain
     * @param shutdownCtrl Enable or shutdown output
     */
    void write(uint16_t value, 
               const OutputChannel& channel,
               const uint8_t bufferControl,
               const uint8_t outputGain,
               const uint8_t shutdownCtrl);
};

#endif