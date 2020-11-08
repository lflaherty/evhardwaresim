#ifndef GPIOINTERFACE_H
#define GPIOINTERFACE_H

#include "DataStore.h"
#include "SimObject.h"

#include <iostream>
#include <cstdint>

class GPIOInterface : public SimObject
{
private:
    /**
     * Mapped GPIO memory pointer
     */
    volatile uint32_t* m_map;

    /**
     * Used to only allow one GPIO call at a time for thread safety.
     */
    pthread_mutex_t m_lock;

public:
    GPIOInterface(DataStore& dataStore);

    virtual void init();

    virtual void step(unsigned long dt);

    /**
     * Definition of Pin structure
     */
    struct Pin {
        const char *name;   // readable name of pin, i.e.: "GPIO1_21", see beaglebone user guide
        const unsigned int gpio_bank; // which of the four gpio banks is this pin in, i.e.: GPIO1, r 0x4804C000 
        const uint8_t gpio; // pin number on the am335x processor
        const uint8_t bank_id; // pin number within each bank, should be 0-31
        const char *mux;    // file name for setting mux
        const uint8_t eeprom; // position in eeprom
    };

    enum Direction {
        GPIO_IN,
        GPIO_OUT
    };

    /**
     * @brief Initialize a pin for GPIO use
     * @param pin Pin to initialize
     * @param direction Input or Output
     */
    void initPin(const Pin* pin, Direction direction);

    /**
     * @brief Write a logic 1 or 0 on a GPIO pin
     * @param pin Pin to write to
     * @param value 1 or 0 to output on pin
     */
    void write(const Pin* pin, bool value);

    /**
     * @brief Read a logic 1 or 0 applied to a GPIO pin
     * @param pin Pin to read from
     * @return 1 or 0 applied to pin
     */
    bool read(const Pin* pin);

    /**
     * Declare BB GPIO pins
     */
    static const Pin USR0;
    static const Pin USR1;
    static const Pin USR2;
    static const Pin USR3;
    static const Pin P8_3;
    static const Pin P8_4;
    static const Pin P8_5;
    static const Pin P8_6;
    static const Pin P8_7;
    static const Pin P8_8;
    static const Pin P8_9;
    static const Pin P8_10;
    static const Pin P8_11;
    static const Pin P8_12;
    static const Pin P8_13;
    static const Pin P8_14;
    static const Pin P8_15;
    static const Pin P8_16;
    static const Pin P8_17;
    static const Pin P8_18;
    static const Pin P8_19;
    static const Pin P8_20;
    static const Pin P8_21;
    static const Pin P8_22;
    static const Pin P8_23;
    static const Pin P8_24;
    static const Pin P8_25;
    static const Pin P8_26;
    static const Pin P8_27;
    static const Pin P8_28;
    static const Pin P8_29;
    static const Pin P8_30;
    static const Pin P8_31;
    static const Pin P8_32;
    static const Pin P8_33;
    static const Pin P8_34;
    static const Pin P8_35;
    static const Pin P8_36;
    static const Pin P8_37;
    static const Pin P8_38;
    static const Pin P8_39;
    static const Pin P8_40;
    static const Pin P8_41;
    static const Pin P8_42;
    static const Pin P8_43;
    static const Pin P8_44;
    static const Pin P8_45;
    static const Pin P8_46;

    static const Pin P9_11;
    static const Pin P9_12;
    static const Pin P9_13;
    static const Pin P9_14;
    static const Pin P9_15;
    static const Pin P9_16;
    static const Pin P9_17;
    static const Pin P9_18;
    static const Pin P9_19;
    static const Pin P9_20;
    static const Pin P9_21;
    static const Pin P9_22;
    static const Pin P9_23;
    static const Pin P9_24;
    static const Pin P9_25;
    static const Pin P9_26;
    static const Pin P9_27;
    static const Pin P9_28;
    static const Pin P9_29;
    static const Pin P9_30;
    static const Pin P9_31;
    static const Pin P9_33;
    static const Pin P9_35;
    static const Pin P9_36;
    static const Pin P9_37;
    static const Pin P9_38;
    static const Pin P9_39;
    static const Pin P9_40;
    static const Pin P9_41;
    static const Pin P9_42;
};

#endif