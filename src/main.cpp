#include <Arduino.h> 
#include <SPI.h> 
#include <LoRa.h> 
#include <MKRWAN.h> 
#include <Wire.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AHTX0.h>
#include <Arduino_MKRGPS.h>

const int timezone_minutes = 240; 
const int timezone_hours = timezone_minutes / 60; 

LoRaModem modem; 
Adafruit_AHTX0 aht;
unsigned long delayTime;

String appEui = "0000000000000000"; 
String appKey = "46C1A645B7200EBB6A3E3D7B78CF5011"; 

float temp; 
float hum; 
float latitude;
float longitude;
float altitude;
float speed;
int satellites;

void setup() {
  Serial.begin(9600);

  if (!modem.begin(US915)) {
    Serial.println("Failed to start module");
    while (1); 
  }

  int connected = modem.joinOTAA(appEui, appKey);
  while (!connected) {
    Serial.println("Retry...");
    if (!modem.joinOTAA(appEui, appKey)) {
      Serial.println("Fail");
    } else {
      break;
    }
  }

  Serial.println("Adafruit AHT20 demo!");

  if (!aht.begin()) {
    Serial.println("Could not find AHT20? Check wiring");
    while (1);
  }
  Serial.println("AHT20 found");

  // if (!GPS.begin()) {
  //   Serial.println("Failed to initialize GPS!");
  //   while (1);
  // }
}

void loop() {
  // if (GPS.available()) {
  //   latitude   = GPS.latitude();
  //   longitude  = GPS.longitude();
  //   altitude   = GPS.altitude();
  //   speed      = GPS.speed();
  //   satellites = GPS.satellites();

    sensors_event_t humEvent, tempEvent;
    aht.getEvent(&humEvent, &tempEvent); 
    temp = tempEvent.temperature;
    hum = humEvent.relative_humidity;

    String payload = String(temp, 2) + "," + String(hum, 2);
    // String payload = String(latitude, 6) + "," + String(longitude, 6) + "," +
    //                  String(temp, 2) + "," + String(hum, 2);
    
    Serial.print("\n===================\n");
    Serial.print("DATA SENT\n");
    Serial.print("===================\n");
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.println(" %");

    modem.beginPacket();
    modem.print(payload);
    if (modem.endPacket() == false) {
      Serial.println("Error sending packet");
    }

    delay(5000);
  }


/*--------------------------------------------------------------------*/

/*
  First Configuration
  This sketch demonstrates the usage of MKR WAN 1300/1310 LoRa module.
  This example code is in the public domain.
*/

// #include <Arduino.h>
// #include <MKRWAN.h>
// #include <Wire.h>
// #include <Adafruit_BusIO_Register.h>
// #include <Adafruit_Sensor.h>

// LoRaModem modem;

// // Uncomment if using the Murata chip as a module
// // LoRaModem modem(Serial1);

// String appEui;
// String appKey;
// String devAddr;
// String nwkSKey;
// String appSKey;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);
//   while (!Serial);
//   Serial.println("Welcome to MKR WAN 1300/1310 first configuration sketch");
//   Serial.println("Register to your favourite LoRa network and we are ready to go!");
//   // change this to your regional band (eg. US915, AS923, ...)
//   if (!modem.begin(US915)) {
//     Serial.println("Failed to start module");
//     while (1) {}
//   };
//   Serial.print("Your module version is: ");
//   Serial.println(modem.version());
//   if (modem.version() != ARDUINO_FW_VERSION) {
//     Serial.println("Please make sure that the latest modem firmware is installed.");
//     Serial.println("To update the firmware upload the 'MKRWANFWUpdate_standalone.ino' sketch.");
//   }
//   Serial.print("Your device EUI is: ");
//   Serial.println(modem.deviceEUI());

//   int mode = 0;
//   while (mode != 1 && mode != 2) {
//     Serial.println("Are you connecting via OTAA (1) or ABP (2)?");
//     while (!Serial.available());
//     mode = Serial.readStringUntil('\n').toInt();
//   }

//   int connected;
//   if (mode == 1) {
//     Serial.println("Enter your APP EUI");
//     while (!Serial.available());
//     appEui = Serial.readStringUntil('\n');

//     Serial.println("Enter your APP KEY");
//     while (!Serial.available());
//     appKey = Serial.readStringUntil('\n');

//     appKey.trim();
//     appEui.trim();

//     connected = modem.joinOTAA(appEui, appKey);
//   } else if (mode == 2) {

//     Serial.println("Enter your Device Address");
//     while (!Serial.available());
//     devAddr = Serial.readStringUntil('\n');

//     Serial.println("Enter your NWS KEY");
//     while (!Serial.available());
//     nwkSKey = Serial.readStringUntil('\n');

//     Serial.println("Enter your APP SKEY");
//     while (!Serial.available());
//     appSKey = Serial.readStringUntil('\n');

//     devAddr.trim();
//     nwkSKey.trim();
//     appSKey.trim();

//     connected = modem.joinABP(devAddr, nwkSKey, appSKey);
//   }

//   if (!connected) {
//     Serial.println("Something went wrong; are you indoor? Move near a window and retry");
//     while (1) {}
//   }

//   delay(5000);

//   int err;
//   modem.setPort(3);
//   modem.beginPacket();
//   modem.print("HeLoRA world!");
//   err = modem.endPacket(true);
//   if (err > 0) {
//     Serial.println("Message sent correctly!");
//   } else {
//     Serial.println("Error sending message :(");
//   }
// }

// void loop() {
//   while (modem.available()) {
//     Serial.write(modem.read());
//   }
//   modem.poll();
// }
