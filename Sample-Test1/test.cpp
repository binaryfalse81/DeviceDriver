#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../Project1/DeviceDriver.cpp"

using namespace std;
using namespace testing;

class FlashMock : public FlashMemoryDevice {
public:
    MOCK_METHOD(unsigned char, read, (long address), (override));
    MOCK_METHOD(void, write, (long address, unsigned char data), (override));

protected:
    void SetUp() {
        //»ý·«
    }
};

class DriverFixture : public testing::Test {
public:
    NiceMock<FlashMock> mk;
    DeviceDriver dd{ &mk };
    Application app{ &dd };
};

TEST_F(DriverFixture, ReadFive) {
    EXPECT_CALL(mk, read).Times(5);
    dd.read(0xA);
}

TEST_F(DriverFixture, ReadException) {
    EXPECT_CALL(mk, read(0x55))
        .WillOnce(Return(0xA))
        .WillOnce(Return(0xA))
        .WillOnce(Return(0xA))
        .WillOnce(Return(0xA))
        .WillOnce(Return(0xB));

    EXPECT_THROW({
        dd.read(0x55);
        }, std::exception);
}

TEST_F(DriverFixture, WriteBeforeRead) {
    EXPECT_CALL(mk, read(0xDD))
        .Times(1)
        .WillRepeatedly(Return(0xFF));

    dd.write(0xDD, 0x72);
}

TEST_F(DriverFixture, blankException) {
    EXPECT_CALL(mk, read(0xDD))
        .WillRepeatedly(Return(0xFA));

    EXPECT_THROW({
        dd.write(0xDD, 0x77);
        }, std::exception);
}

TEST_F(DriverFixture, ApplicationRead) {
    //0x00 ~ 0x04 (5°³)
    EXPECT_CALL(mk, read(_))
        .Times(25);

    app.readAndPrint(0x00, 0x04);
}

TEST_F(DriverFixture, ApplicationWrite) {
    EXPECT_CALL(mk, read(_))
        .Times(5)
        .WillRepeatedly(Return(0xFF));

    app.writeAll(0x77);
}
