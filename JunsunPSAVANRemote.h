// JunsunPSAVANRemote.h

#ifndef _JUNSUNVANREMOTE_h
#define _JUNSUNVANREMOTE_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

enum JunsunPSAVANButton
{
    VolumeUp = 0x01,
    VolumeDown = 0x02,
    NextTrack = 0x03,
    PreviousTrack = 0x04,
    Source = 0x05,
    //PreviousAlbum = 0x06,
    Mute = 0x07,
    //NextAlbum = 0x07,
    TripComputer = 0x20
};

enum JunsunPSAVANFunctionId
{
    CarStatus = 0x01,
    ButtonCode = 0x20
};

typedef struct {
    uint8_t unknown0    : 1; // bit 0
    uint8_t unknown1    : 1; // bit 1
    uint8_t hood        : 1; // bit 2
    uint8_t trunk       : 1; // bit 3
    uint8_t rear_left   : 1; // bit 4
    uint8_t rear_right  : 1; // bit 5
    uint8_t front_left  : 1; // bit 6
    uint8_t front_right : 1; // bit 7
} DoorStatusStruct;

typedef union {
    DoorStatusStruct status;
    uint8_t asByte;
} DoorStatus;


constexpr uint8_t MAX_JUNSUN_MESSAGE_LENGTH = 17;

/*
 * This is an implementation for remote controlling 8227L based Chinese Android players. These are called Junsun, Mekede, etc.
 * In order to make it work, you need to select 108. Peugeot206/207/307 from the CAN Type settings.
 * This library is tied to the Android unit and not to the car's model. So you don't have to have a Peugeot car to make this library work.
 * You can interface any car as long as you implement whatever protocol it uses.
 */
class JunsunPSAVANRemote
{
private:
    #define JS_ARRAY_SIZE(x)  (sizeof(x) / sizeof((x)[0]))
    Stream* serialPort;
    uint8_t outputBuffer[MAX_JUNSUN_MESSAGE_LENGTH];

    void SendData(uint8_t functionId, uint8_t message[], uint8_t messageLength);

public:
    /*
     * Serial should be an initialized Serial port with the following setup:
     * Baud      : 38400
     * Start bits: 1
     * Data bits : 8
     * Stop bits : 1
     */
    JunsunPSAVANRemote(Stream &serial);
    ~JunsunPSAVANRemote();

    void SendButtonCode(JunsunPSAVANButton button);
    void SendCarInfo(uint8_t averageSpeed, uint16_t range, uint16_t distanceTravelled, float instantConsumption, float avgConsumption, int8_t externalTemperature, DoorStatus doorStatus);
};
#endif
