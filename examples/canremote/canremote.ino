#include <JunsunPSACANRemote.h>

#define RXD2 16
#define TXD2 17

JunsunPSACANRemote* remote;

void setup()
{
    Serial.begin(500000);
    Serial2.begin(19200, SERIAL_8N1, RXD2, TXD2);

    remote = new JunsunPSACANRemote(Serial2);
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
        Serial.println("Next album");
        remote->SendButtonCode(NextAlbum);
    }
    if (inChar == '4') {
        Serial.println("Previous album");
        remote->SendButtonCode(NextAlbum);
    }
    if (inChar == '5') {
        Serial.println("Volume up");
        remote->SendButtonCode(VolumeUp);
    }
    if (inChar == '6') {
        Serial.println("Volume down");
        remote->SendButtonCode(VolumeDown);
    }
    if (inChar == '7') {
        Serial.println("Mute");
        remote->SendButtonCode(Mute);
    }
    if (inChar == '8') {
        Serial.println("TripButton");
        remote->SendButtonCode(TripComputer);
    }
    if (inChar == 'q') {
        Serial.println("Temperature 23");
        remote->SendTemperature(23);
    }
    if (inChar == 'w') {
        Serial.println("Temperature -23");
        remote->SendTemperature(-23);
    }
    if (inChar == 'e') {
        Serial.println("TripCar");
        remote->SendTripDataCar(1234, 15.9, 3210);
    }
    if (inChar == 'r') {
        Serial.println("Trip1");
        remote->SendTripData1(2345, 11.7, 159);
    }
    if (inChar == 't') {
        Serial.println("Trip2");
        remote->SendTripData2(4685, 4.2, 66);
    }
}
