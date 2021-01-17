// Adapted from https://github.com/ehayon/BeagleBone-GPIO/blob/master/src/gpio.c

#include "GPIOInterface.h"

#include <iostream>
#include <cstdint>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>


/* ***** Define GPIO Pins ***** */
const GPIOInterface::Pin GPIOInterface::P9_23 = { .gpio = 49 };
const GPIOInterface::Pin GPIOInterface::P9_25 = { .gpio = 117 };
const GPIOInterface::Pin GPIOInterface::P9_27 = { .gpio = 115 };
const GPIOInterface::Pin GPIOInterface::P9_28 = { .gpio = 113 };
const GPIOInterface::Pin GPIOInterface::P9_29 = { .gpio = 111 };
const GPIOInterface::Pin GPIOInterface::P9_30 = { .gpio = 112 };


GPIOInterface::GPIOInterface(DataStore& dataStore)
    : SimObject(dataStore, "GPIO Interface", 1)  // 1Hz
{
    std::cout << "[GPIO Interface] Beginning device init" << std::endl;

    std::cout << "[GPIO Interface] Completed device init" << std::endl;
}

void GPIOInterface::init()
{
    std::cout << "[" << getName() << "] init" << std::endl;
    // No variables to be initialized
}

void GPIOInterface::step(unsigned long)
{
    // Nothing to be done here for this sim device
}

void GPIOInterface::initPin(const Pin* pin, Direction direction)
{
    std::cout << "[GPIO Interface] Configuring pin " << pin->gpio << std::endl;

    pthread_mutex_lock(&m_lock);

    if (m_pinInitialized.find(pin->gpio) == m_pinInitialized.end()) {
        // export
        int fd, len;
        char buf[64];

        // fd = open("/sys/class/gpio/export", O_WRONLY);
        // if (fd < 0) {
        //     perror("[GPIO Interface] Unable to open /sys/class/gpio/export");
        //     close(fd);
        //     pthread_mutex_unlock(&m_lock);
        //     return;
        // }

        // len = snprintf(buf, sizeof(buf), "%d", pin->gpio);
        // write(fd, buf, len);
        // close(fd);    

        // set direction
        snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", pin->gpio);

        fd = open(buf, O_WRONLY);
        if (fd < 0) {
            perror("[GPIO Interface] Unable to open gpio/direction");
            close(fd);
            pthread_mutex_unlock(&m_lock);
            return;
        }

        if (direction == GPIO_OUT) {
            write(fd, "out", 4);
        } else {
            write(fd, "in", 3);
        }

        close(fd);
    }

    m_pinInitialized[pin->gpio] = true;

    pthread_mutex_unlock(&m_lock);

    std::cout << "[GPIO Interface] Completed configuring pin" << std::endl;
}

void GPIOInterface::writePin(const Pin* pin, bool value)
{
    pthread_mutex_lock(&m_lock);

	int fd;
	char buf[64];

	snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", pin->gpio);

    // TODO: do I really have to open/close each time? Open once and store the fd?
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("[GPIO Interface] Unable to open gpio/value");
	    close(fd);
        pthread_mutex_unlock(&m_lock);
		return;
	}

	if (value == 0) {
		write(fd, "0", 2);
    } else {
		write(fd, "1", 2);
    }

	close(fd);

    pthread_mutex_unlock(&m_lock);
}

bool GPIOInterface::readPin(const Pin* pin)
{
    bool value;

    pthread_mutex_lock(&m_lock);

	int fd;
	char buf[64];
	char ch;

	snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", pin->gpio);

	fd = open(buf, O_RDONLY);
	if (fd < 0) {
		perror("GPIO Interface] Unable to open gpio/value");
	    close(fd);
        pthread_mutex_unlock(&m_lock);
		return false;
	}

	read(fd, &ch, 1);

	if (ch != '0') {
		value = 1;
	} else {
		value = 0;
	}

	close(fd);

    pthread_mutex_unlock(&m_lock);

    return value;
}