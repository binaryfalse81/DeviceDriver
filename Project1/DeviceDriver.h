#pragma once
#include "FlashMemoryDevice.h"

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice *hardware);
    int read(long address);
    void write(long address, int data);

protected:
    FlashMemoryDevice *m_hardware;

private:
    const int RETRY_TEST_COUNT = 4;

    void readTestMustBlank(long address);
    void tryTest(long address, int readValue);
};
