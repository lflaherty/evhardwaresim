#ifndef PrintTask_H
#define PrintTask_H

#include "DataStore.h"
#include "SimObject.h"

#include "CANInterface.h"
#include "UARTInterface.h"

#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>

class PrintTask : public SimObject
{
private:
    static void canCallback(void* obj, uint32_t msgId, uint8_t* data, size_t);
    static void uartCallback(void* obj, uint8_t data);

    /**
     * CAN bus access
     */
    std::shared_ptr<CANInterface> m_can;

    /**
     * UART access
     */
    std::shared_ptr<UARTInterface> m_uart;

    struct CANFrame {
        uint32_t msgId;
        uint8_t data[8];
        size_t len;
    };

    /**
     * Stores history of received CAN messages
     */
    std::vector<CANFrame> m_receivedMsgs;

    /**
     * Stores history of received UART bytes
     */
    std::vector<uint8_t> m_receivedUart;

    bool m_clearOnPeriodicPrint;

    template <typename T>
    void printVariable(std::string name, T value);

    template <typename T>
    void printVariableRatio(std::string name, T value, T maxValue);

public:
    PrintTask(
        DataStore& dataStore,
        std::shared_ptr<CANInterface> canInterface,
        std::shared_ptr<UARTInterface> uartInterface);

    virtual void init();

    virtual void step(unsigned long dt);

    // Sets whether to clear the console before each 1s print
    void setClearOnPeriodicPrint(const bool clear);
};

template <typename T>
void PrintTask::printVariable(std::string name, T value)
{
    std::cout << "\t\t" << name << "\t" << value << std::endl;
}

template <typename T>
void PrintTask::printVariableRatio(std::string name, T value, T maxValue)
{
    float ratio = 100.0f * static_cast<float>(value)/static_cast<float>(maxValue);

    std::cout << "\t\t" << name << "\t" << value;
    std::cout << "\t" << std::fixed << std::setprecision(2) << ratio << "%" << std::endl;
}

#endif