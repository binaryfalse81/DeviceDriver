#include "DeviceDriver.h"
#include <stdexcept>
using namespace std;


class WriteException : public exception
{
public:
    explicit WriteException(char const* _Message)
        : exception(_Message)
    {

    }
};

class ReadException : public exception
{
public:
    explicit ReadException(char const* _Message)
        : exception(_Message)
    {

    }
};

DeviceDriver::DeviceDriver(FlashMemoryDevice *hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    int data = (int)(m_hardware->read(address));
    for (int i = 1; i < READ_RETRY_CNT; i++)
    {
        if (data != (int)(m_hardware->read(address)))
        {
            throw ReadException("read exception");
        }
    }
    return data;
}

void DeviceDriver::write(long address, int data)
{
    int prevData = (int)(m_hardware->read(address));
    if (prevData != ERASE_DATA)
    {
        throw WriteException("write exception");
    }
    m_hardware->write(address, (unsigned char)data);
}
