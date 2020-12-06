#ifndef GPIOINTERFACE_H
#define GPIOINTERFACE_H

#include "DataStore.h"
#include "SimObject.h"

#include <iostream>
#include <cstdint>
#include <map>

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

    /**
     * Used to determine whether pins have been initialized
     * 
     * Maps GPIO pin num -> initialized? 
     */
    std::map<unsigned int, bool> m_pinInitialized;

public:
    GPIOInterface(DataStore& dataStore);

    virtual void init();

    virtual void step(unsigned long dt);

    /**
     * Definition of Pin structure
     */
    struct Pin {
        unsigned int gpio;   // GPIO number for this pin
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
    void writePin(const Pin* pin, bool value);

    /**
     * @brief Read a logic 1 or 0 applied to a GPIO pin
     * @param pin Pin to read from
     * @return 1 or 0 applied to pin
     */
    bool readPin(const Pin* pin);

    /**
     * Declare BB GPIO pins
     */
    static const Pin P9_23;
    static const Pin P9_25;
    static const Pin P9_27;
    static const Pin P9_28;
    static const Pin P9_29;
    static const Pin P9_30;
};

#endif