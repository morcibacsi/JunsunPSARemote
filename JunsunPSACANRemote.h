// JunsunPSACANRemote.h

#ifndef _JunsunPSACANRemote_h
#define _JunsunPSACANRemote_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

enum JunsunPSAButton
{
    Source = 0x11,
    NextTrack = 0x12,
    PreviousTrack = 0x13,
    VolumeUp = 0x14,
    VolumeDown = 0x15,
    Mute = 0x16,
    NextAlbum = 0x17,
    PreviousAlbum = 0x18,
    TripComputer = 0x20
};

constexpr uint8_t MAX_JUNSUN_MESSAGE_LENGTH = 13;

/*
 * This is an implementation for remote controlling 8227L based Chinese Android players. These are called Junsun, Mekede, etc.
 * In order to make it work, you need to select 127. PSA(Rcz) from the CAN Type settings.
 * This library is tied to the Android unit and not to the car's model. So you don't have to have a Peugeot car to make this library work.
 * You can interface any car as long as you implement whatever protocol it uses.
 */
class JunsunPSACANRemote
{
private:
    #define JS_ARRAY_SIZE(x)  (sizeof(x) / sizeof((x)[0]))
    Stream* serialPort;
    uint8_t outputBuffer[MAX_JUNSUN_MESSAGE_LENGTH];

    void SendData(uint8_t message[], uint8_t messageLength);

public:
    /*
     * Serial should be an initialized Serial port with the following setup:
     * Baud      : 19200
     * Start bits: 1
     * Data bits : 8
     * Stop bits : 1
     */
    JunsunPSACANRemote(Stream &serial);
    ~JunsunPSACANRemote();

    void SendButtonCode(JunsunPSAButton button);
    void SendSpeed(uint8_t speed);
    void SendTripDataCar(uint16_t icon1Data, double icon2Data, uint16_t icon3Data);
    void SendTripData1(uint16_t icon1Data, double icon2Data, uint8_t icon3Data);
    void SendTripData2(uint16_t icon1Data, double icon2Data, uint8_t icon3Data);
    void SendTemperature(int8_t temperature);
};


#endif

