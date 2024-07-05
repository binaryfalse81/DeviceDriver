#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../Project1/DeviceDriver.cpp"

using namespace std;
using namespace testing;

class FlashMock : public FlashMemoryDevice
{
public:
    MOCK_METHOD(unsigned char, read, (long address), (override));
    MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

TEST(DEVICE_DRIVER_TEST, ReadFive)
{
    FlashMock mk;
    DeviceDriver dd{ &mk };
    EXPECT_CALL(mk, read).Times(5);
    dd.read(0xA);
}

TEST(DEVICE_DRIVER_TEST, ReadException)
{
    FlashMock mk;
    DeviceDriver dd{ &mk };
    EXPECT_CALL(mk, read)
        .WillOnce(Return(1))
        .WillOnce(Return(2));

    EXPECT_THROW({ dd.read(0xA); }, exception);
}

TEST(DEVICE_DRIVER_TEST, Write)
{
    FlashMock mk;
    DeviceDriver dd{ &mk };
    EXPECT_CALL(mk, read(0xA))
        .Times(1)
        .WillRepeatedly(Return(0xFF));

    dd.write(0xA, 10);
}

TEST(DEVICE_DRIVER_TEST, WriteException)
{
    FlashMock mk;
    DeviceDriver dd{ &mk };
    EXPECT_CALL(mk, read(0xB))
        .Times(1)
        .WillRepeatedly(Return(0xBB));

    EXPECT_THROW({ dd.write(0xB, 10); }, exception);
}
