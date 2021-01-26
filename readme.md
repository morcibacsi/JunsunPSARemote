# Junsun Android Head unit remote (PSA)

### What is it ?

There are various Android based head units based on the MTK 8227l chipset coming from China. These head units are having the brand names like MEKEDE, YT9218, and Junsun. They usually come with an CAN bus adapter which helps you to connect your new head unit to your car and have acces to the remote on your steering wheel, and have support for the trip computer data on your Android device etc. 
This project is an **Arduino library** which was created by reverse engineering the protocol between the head unit and the CAN bus adapter created by Raise for PSA cars (Peugeot/Citroen).
The CAN box adapter has much more functionality, like switching to the reverse camera input, making sound for the parking radar, dimming the display when the side lights are on and who knows what else. These functions are using separate wires so they aren't available as commands.

### Why ?

Because it is fun. But other than that if you forgot to order the CAN bus adapter, or you are looking for a DIY project I am here to help. You don't need to have a Peugeot/Citroen car for this library, as it is written for the headunit. You can interface any car as long as you implement whatever protocol it uses.  For example you may have a Ford car where you implement the CAN bus parsing, but you use this library telling the headunit that you have a Peugeot.

### The protocol

#### CAN box protocol

When you select CAN Type **127. PSA(Rzc)** from the settings the headunit will use a **19200** baud serial port with a 8 databits 1 stop bit and no parity to communicate with the CAN box. It uses a simple protocol to send commands.

A sample message:

    0xFD 0x04 0x02 0x11 0x17
     │    │   └───┼───┘  └────── checksum (sum of the previous bytes excluding the header, can overflow)
     │    │       └───────────── data bytes
     │    └───────────────────── length of the message including this byte and the data and checksum 
     │                           bytes
     └────────────────────────── header

#### VAN box protocol

VAN bus was used in Peugeot 206(+), 307 (2001-2005), 406,1007, Citroen Xsara, C3, C5... etc cars between 2001-2005.
When you select CAN Type **108. Peugeot206/207/307** from the settings the headunit will use a **38400** baud serial port with a 8 databits 1 stop bit and no parity to communicate with the "CAN box". It uses a simple protocol to send commands.

    0x2E 0x01 0x02 0x01 0x01 0xDB
     │    │    │   └───┼───┘  └────── checksum (0xFF - sum of the previous bytes excluding the header, can overflow) 
     │    │    │       └───────────── data bytes
     │    │    └───────────────────── length of the payload
     │    └────────────────────────── function id
     └─────────────────────────────── header

### Usage
I attached two simple sketches which can be found in the examples folder. 
If you use the JunsunPSA**C**ANRemote library then need to set CAN type to **127. PSA(Rzc)** on the head unit to make it work.
![schema](https://github.com/morcibacsi/JunsunPSARemote/raw/main/extras/can_setup.jpg)

A basic sketch with JunsunPSA**C**ANRemote is like this:

    #include <JunsunPSACANRemote.h>
    
    #define RXD2 16
    #define TXD2 17

    JunsunPSACANRemote* remote;
    void setup()
    {
        // we use the Serial2 of the ESP32 to communicate with the head unit
        Serial2.begin(19200, SERIAL_8N1, RXD2, TXD2);
        // we instantiate the library
        remote = new JunsunPSACANRemote(Serial2);
    }

    void loop()
    {
        remote->SendButtonCode(VolumeUp);
        delay(3000);
        remote->SendButtonCode(VolumeDown);
        remote->SendTemperature(23);
        remote->SendTripDataCar(1234, 15.9, 3210);
        remote->SendTripData1(2345, 11.7, 159);
        remote->SendTripData2(4685, 4.2, 66);
    }

If you use the JunsunPSA**V**ANRemote library then need to set CAN type to **108. Peugeot206/207/307** on the head unit to make it work.
![schema](https://github.com/morcibacsi/JunsunPSARemote/raw/main/extras/van_setup.jpg)

A basic sketch with JunsunPSA**V**ANRemote is like this:

    #include <JunsunPSAVANRemote.h>
    
    #define RXD2 16
    #define TXD2 17

    JunsunPSAVANRemote* remote;
    void setup()
    {
        // we use the Serial2 of the ESP32 to communicate with the head unit
        Serial2.begin(38400, SERIAL_8N1, RXD2, TXD2);
        // we instantiate the library
        remote = new JunsunPSAVANRemote(Serial2);
    }

    void loop()
    {
        remote->SendButtonCode(VolumeUp);
        delay(6000);
        remote->SendButtonCode(VolumeDown);
        delay(6000);
        
        DoorStatus ds;
        ds.status.front_left = 1;
        remote->SendCarInfo(47, 653, 1549, 6, 8, -27, ds);
        delay(6000);

        ds.status.front_left = 0;
        remote->SendCarInfo(47, 653, 1549, 6, 8, -27, ds);
        delay(6000);
    }

You can watch a short video about it here:
[![WATCH IT ON YOUTUBE](https://github.com/morcibacsi/JunsunPSARemote/raw/main/extras/youtube_preview.jpg)](https://www.youtube.com/watch?v=MtLH9LZdBNc "Watch it on YouTube")

The Car Info application displaying our values:
![van_features](https://github.com/morcibacsi/JunsunPSARemote/raw/main/extras/van_features.jpg)

### Schema

The schema and the example uses an ESP32 devboard, but the library should work with other Arduino compatible boards as well.

![schema](https://github.com/morcibacsi/JunsunPSARemote/raw/main/extras/schema.jpg)

### Components

The components needed for the project to work in your car is the following:
- The Android head unit
- An ESP32 dev board

### Liability

I cannot take any responsibility if something goes wrong if you build it and install it in your car. So use it at your own risk.
