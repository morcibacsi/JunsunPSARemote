#include <JunsunPSAVANRemote.h>

#define RXD2 16
#define TXD2 17

JunsunPSAVANRemote* remote;

void setup()
{
    Serial.begin(500000);
    Serial2.begin(38400, SERIAL_8N1, RXD2, TXD2);

    remote = new JunsunPSAVANRemote(Serial2);
}

void loop()
{
    uint8_t inChar = (uint8_t)Serial.read();

    if (inChar == '0') {
        Serial.println("Source");
        remote->SendButtonCode(Source);
    }
    if (inChar == '1') {
        Serial.println("Next track");
        remote->SendButtonCode(NextTrack);
    }
    if (inChar == '2') {
        Serial.println("Previous track");
        remote->SendButtonCode(PreviousTrack);
    }
    if (inChar == '3') {
        Serial.println("Volume up");
        remote->SendButtonCode(VolumeUp);
    }
    if (inChar == '4') {
        Serial.println("Volume down");
        remote->SendButtonCode(VolumeDown);
    }
    if (inChar == '5') {
        Serial.println("Mute");
        remote->SendButtonCode(Mute);
    }
    if (inChar == '6') {
        Serial.println("TripButton");
        remote->SendButtonCode(TripComputer);
    }
    if (inChar == 'q') {
        Serial.println("Front left door open");

        DoorStatus ds;
        ds.status.front_left = 1;
        remote->SendCarInfo(47, 653, 1549, 6, 8, -27, ds);
    }
    if (inChar == 'w') {
        Serial.println("Front left door closed");

        DoorStatus ds;
        ds.status.front_left = 0;
        remote->SendCarInfo(47, 653, 1549, 6, 8, -27, ds);
    }
}
