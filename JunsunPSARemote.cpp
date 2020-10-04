#include "JunsunPSARemote.h"

JunsunPSARemote::JunsunPSARemote(Stream& serial)
{
    serialPort = &serial;
}

void JunsunPSARemote::SendButtonCode(JunsunPSAButton button)
{
    uint8_t data[] = { 0x02, (uint8_t) button };
    SendData(data, JS_ARRAY_SIZE(data));
}

void JunsunPSARemote::SendSpeed(uint8_t speed)
{
    uint16_t speedData = speed * 100;

    uint8_t part1 = lowByte(speedData);
    uint8_t part2 = highByte(speedData);

    uint8_t data[] = { 0x7D, 0x03, part2, part1, 0x00, 0x00 };
    SendData(data, JS_ARRAY_SIZE(data));
}

void JunsunPSARemote::SendTripDataCar(uint16_t icon1Data, double icon2Data, uint16_t icon3Data)
{
    // icon1 max = 2000
    // icon3 max = 6000

    uint8_t icon1_part1 = highByte(icon1Data);
    uint8_t icon1_part2 = lowByte(icon1Data);

    uint8_t icon3_part1 = highByte(icon3Data);
    uint8_t icon3_part2 = lowByte(icon3Data);


    uint8_t icon2_part1 = 0x00;
    uint8_t icon2_part2 = 0x00;
    if (icon2Data < 3.9)
    {
        icon2_part1 = 0xFF;
    }
    else
    {
        icon2_part2 = round(1000.0 / icon2Data);
    }

    uint8_t data[] = { 0x33, icon2_part1, icon2_part2, icon1_part1, icon1_part2, icon3_part1, icon3_part2, 0x00, 0x00, 0x00 };
    SendData(data, JS_ARRAY_SIZE(data));
}

void JunsunPSARemote::SendTripData1(uint16_t icon1Data, double icon2Data, uint8_t icon3Data)
{
    uint8_t icon1_part1 = highByte(icon1Data);
    uint8_t icon1_part2 = lowByte(icon1Data);

    uint8_t icon2_part1 = 0x00;
    uint8_t icon2_part2 = 0x00;

    if (icon2Data < 3.9)
    {
        icon2_part1 = 0xFF;
    }
    else
    {
        icon2_part2 = round(1000.0 / icon2Data);
    }

    //uint8_t data[] = { 0x34, icon2_part1, icon2_part2, 0x00, icon1_part1, icon1_part2, icon3Data };
    uint8_t data[] = { 0x34, icon2_part1, icon2_part2, 0x00, icon3Data, icon1_part1, icon1_part2 };
    SendData(data, JS_ARRAY_SIZE(data));
}

void JunsunPSARemote::SendTripData2(uint16_t icon1Data, double icon2Data, uint8_t icon3Data)
{
    uint8_t icon1_part1 = highByte(icon1Data);
    uint8_t icon1_part2 = lowByte(icon1Data);

    uint8_t icon2_part1 = 0x00;
    uint8_t icon2_part2 = 0x00;

    if (icon2Data < 3.9)
    {
        icon2_part1 = 0xFF;
    }
    else
    {
        icon2_part2 = round(1000.0 / icon2Data);
    }

    uint8_t data[] = { 0x35, icon2_part1, icon2_part2, 0x00, icon3Data, icon1_part1, icon1_part2 };
    SendData(data, JS_ARRAY_SIZE(data));
}

void JunsunPSARemote::SendTemperature(int8_t temperature)
{
    // Max temperature which is displayed is 87
    // Min temperature which is displayed is -40

    uint8_t temperatureByte = 0x00;

    if (temperature >= 0)
    {
        temperatureByte = temperature;
    }
    else
    {
        temperatureByte = 0x80 + (-1 * temperature);
    }

    uint8_t data[] = { 0x36, temperatureByte };
    SendData(data, JS_ARRAY_SIZE(data));
}

void JunsunPSARemote::SendData(uint8_t message[], uint8_t messageLength)
{
    /* the commands have the following layout
     *      1st byte: 0xFD
     *      2nd byte: length of the message (including the checkSum + length of the message
     *      data bytes
     *      last byte: checkSum (sum of the individual bytes from 2nd byte)
     *
     *      Example: 0xFD 0x04 0x02 0x13 0x19
     *
     *      0xFD: header
     *      0x04: length of the message: elemCount(0x04, 0x02, 0x13, 0x19)
     *      0x19: checksum = 0x04 + 0x02 + 0x13 (it is a byte so it can overflow)
    */

    // the first element is 0xFD
    outputBuffer[0] = 0xFD;

    // the second is the overall length of the message (messageLength byte + checksum included)
    outputBuffer[1] = messageLength + 2;

    uint8_t checkSum = outputBuffer[1];
    for (int i = 0; i < messageLength; ++i)
    {
        outputBuffer[i + 2] = message[i];
        checkSum += message[i];
    }

    // the last byte is the checksum
    outputBuffer[messageLength + 2] = checkSum;

    serialPort->write(outputBuffer, messageLength + 3);
}
