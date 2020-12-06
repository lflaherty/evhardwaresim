#include "SPIInterface.h"

#include <iostream>
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <stdlib.h>
SPIInterface::SPIInterface(
    DataStore& dataStore, 
    std::shared_ptr<GPIOInterface> gpioInterface,
    const SPIConfig& config)
    : SimObject(dataStore, "SPI Interface", 1),  // 1Hz
      m_gpio(gpioInterface),
      m_errorPrinted(false)
{
    std::cout << "[SPI Interface] Beginning device init" << std::endl;

    int ret = 0;

    // Open SPI device
    m_fd = open(config.device.c_str(), O_RDWR);
    if (m_fd < 0) {
        perror("[SPI Interface] can't open device");
        return;
    }

    // SPI mode
    ret = ioctl(m_fd, SPI_IOC_WR_MODE, &config.mode);
    if (ret == -1) {
        perror("[SPI Interface] can't set spi mode");
        return;
    }

    ret = ioctl(m_fd, SPI_IOC_RD_MODE, &config.mode);
    if (ret == -1) {
        perror("[SPI Interface] can't get spi mode");
        return;
    }

    // Bits per word
    ret = ioctl(m_fd, SPI_IOC_WR_BITS_PER_WORD, &config.bits);
    if (ret == -1) {
        perror("[SPI Interface] can't set bits per word");
        return;
    }

    ret = ioctl(m_fd, SPI_IOC_RD_BITS_PER_WORD, &config.bits);
    if (ret == -1)
        perror("[SPI Interface] can't get bits per word");

    // MSB first
    ret = ioctl(m_fd, SPI_IOC_WR_LSB_FIRST, &config.firstBit);
    if (ret == -1) {
        perror("[SPI Interface] can't set MSB first");
        return;
    }

    ret = ioctl(m_fd, SPI_IOC_RD_LSB_FIRST, &config.firstBit);
    if (ret == -1) {
        perror("[SPI Interface] can't get MSB first");
        return;
    }

    // Max speed (Hz)
    ret = ioctl(m_fd, SPI_IOC_WR_MAX_SPEED_HZ, &config.speed);
    if (ret == -1) {
        perror("[SPI Interface] can't set max speed hz");
        return;
    }

    ret = ioctl(m_fd, SPI_IOC_RD_MAX_SPEED_HZ, &config.speed);
    if (ret == -1) {
        perror("[SPI Interface] can't get max speed hz");
        return;
    }
    
    std::cout << "[SPI Interface] Setup successful" << std::endl;
    std::cout << "[SPI Interface] Mode: " << config.mode << std::endl;
    std::cout << "[SPI Interface] Bits per word: " << config.bits << std::endl;
    std::cout << "[SPI Interface] Max speed: " << config.speed;
    std::cout << " Hz (" << (config.speed / 1000) << " kHz)" << std::endl;
}

void SPIInterface::init()
{
    std::cout << "[" << getName() << "] init" << std::endl;
    // No variables to be initialized
}

void SPIInterface::step(unsigned long)
{
    // Nothing to be done here for this sim device
}

void SPIInterface::transfer(
    uint8_t* rx, uint8_t* tx, uint8_t len, const GPIOInterface::Pin* cs)
{
    int ret;

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,

        .len = len,
        .speed_hz = 0,

        .delay_usecs = 0,
        .bits_per_word = 0,
        .cs_change = 0,
        .tx_nbits = 0,
        .rx_nbits = 0,
        .word_delay_usecs = 0,
        .pad = 0
    };

    // SPI MODE 0 CS toggling
    m_gpio->writePin(cs, false);

    ret = ioctl(m_fd, SPI_IOC_MESSAGE(1), &tr);

    m_gpio->writePin(cs, true);

    if (ret == -1 && !m_errorPrinted) {
        perror("[SPI Interface] Can't send spi message");
        m_errorPrinted = true;
    }
}