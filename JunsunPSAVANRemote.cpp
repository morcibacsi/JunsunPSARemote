#include "JunsunPSAVANRemote.h"

JunsunPSAVANRemote::JunsunPSAVANRemote(Stream& serial)
{
    serialPort = &serial;
}

void JunsunPSAVANRemote::SendButtonCode(JunsunPSAVANButton button)
{
    uint8_t data[] = { (uint8_t) button, 0x01 }; //for press
    //uint8_t data[] = { (uint8_t) button, 0x00 }; // for release?
    SendData(ButtonCode, data, JS_ARRAY_SIZE(data));
}

void JunsunPSAVANRemote::SendCarInfo(uint8_t averageSpeed, uint16_t range, uint16_t distanceTravelled, float instantConsumption, float avgConsumption, int8_t externalTemperature, DoorStatus doorStatus)
{
    uint8_t temperatureByte = 0x00;

    if (externalTemperature >= 0)
    {
        temperatureByte = externalTemperature;
    }
    else
    {
        temperatureByte = 0xFF - (-1*(externalTemperature+1));
    }

    uint16_t avgCons = avgConsumption * 10;
    uint16_t instantCons = instantConsumption * 10;

    uint8_t data[] = { averageSpeed, highByte(range), lowByte(range), highByte(distanceTravelled), lowByte(distanceTravelled), highByte(instantCons), lowByte(instantCons), highByte(avgCons), lowByte(avgCons), temperatureByte, doorStatus.asByte, 0x03, 0xFF };
    SendData(CarStatus, data, JS_ARRAY_SIZE(data));
}

void JunsunPSAVANRemote::SendData(uint8_t functionId, uint8_t message[], uint8_t messageLength)
{
    /* the commands have the following layout
     *      1st byte: 0x2E
     *      2nd byte: function id
     *      3rd byte: length of the payload (excluding the checksum and the first two bytes)
     *      data bytes
     *      last byte: checkSum (0xFF - sum of the individual bytes from 2nd byte)
     *
     *      Example: 0x2E 0x20 0x02 0x01 0x01 0xDB
     *
     *      0x2E: header
     *      0x20: function id
     *      0x02: length of the message: elemCount(0x01, 0x01)
     *      0x01: payload byte1
     *      0x01: payload byte2
     *      0xDB: checksum = 0xFF - (0x20 + 0x02 + 0x01 + 0x01) (it is a byte so it can overflow)
    */

    // the first element is 0x2E
    outputBuffer[0] = 0x2E;

    // the second is the function id
    outputBuffer[1] = functionId;

    // the third is the length of the payload
    outputBuffer[2] = messageLength;

    uint8_t sumOfPayload = outputBuffer[1] + outputBuffer[2];
    for (int i = 0; i < messageLength; ++i)
    {
        outputBuffer[i + 3] = message[i];
        sumOfPayload += message[i];
    }

    // the last byte is the checksum
    outputBuffer[messageLength + 3] = 0xFF - sumOfPayload;

    //for (int i = 0; i < messageLength+4; ++i)
    //{
    //    Serial.print(outputBuffer[i], HEX);
    //    Serial.print(" ");
    //} 
    //Serial.println();

    serialPort->write(outputBuffer, messageLength + 4);
}
