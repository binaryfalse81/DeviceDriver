#include <stdexcept>
#include "DeviceDriver.h"

class ReadFail : public std::exception {};
class WriteFail : public std::exception {};

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    int readValue = (int)(m_hardware->read(address));
    tryTest(address, readValue);

    return readValue;
}

void DeviceDriver::tryTest(long address, int readValue)
{
    for (int i = 0; i < RETRY_TEST_COUNT; i++) {
        int testValue = (int)(m_hardware->read(address));
        if (testValue != readValue) {
            throw ReadFail();
        }
    }
}

void DeviceDriver::write(long address, int data)
{
    readTestMustBlank(address);
    m_hardware->write(address, (unsigned char)data);
}

void DeviceDriver::readTestMustBlank(long address)
{
    int testValue = (int)(m_hardware->read(address));
    if (testValue != 0xFF) {
        throw WriteFail();
    }
}



class Application {
public:
    Application(DeviceDriver* driver) {
        this->driver = driver;
    }

    void readAndPrint(int startAddr, int endAddr) {
        for (int addr = startAddr; addr <= endAddr; addr++) {
            driver->read(addr);
        }
    }

    void writeAll(int value) {
        for (int addr = 0x00; addr <= 0x04; addr++) {
            driver->write(addr, value);
        }
    }

private:
    DeviceDriver* driver;
};