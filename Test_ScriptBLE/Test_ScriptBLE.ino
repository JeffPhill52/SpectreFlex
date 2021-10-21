

// Libraries for Bloothtooth Module
#include <ArduinoBLE.h>

// Bluetooth Initialization and Constants 
BLEService controlService("6f457c49-2d99-4334-b087-b7cd9e62d807");

//Gyro characteristic can be read and notified by remote device
BLEUnsignedCharCharacteristic testCharacteristic("6f457c49-2d99-4334-b087-b7cd9e62d808", BLERead | BLENotify);

// Flex sensor pins
/*
const int FLEX1 = A0;     // Thumb
const int FLEX2 = A1;     // Index
const int FLEX3 = A2;     // Middle
const int FLEX4 = A3;     // Ring
*/

void startBLE(){
  if (!BLE.begin())
  {
    Serial.println("starting BLE failed!");
  }

    /* 
   *  Bluetooth Setup
   */
  BLE.setDeviceName("Spectre Flex");
  BLE.setLocalName("Spectre Flex");
  controlService.addCharacteristic(testCharacteristic);
  BLE.setAdvertisedServiceUuid(controlService.uuid());
  BLE.advertise();
}

void setup() {

  Serial.begin(9600);
  Serial.println("Arduino Started");

  startBLE();
}

void loop() {
  BLEDevice central = BLE.central();

  if (central.connected())
  {
    testCharacteristic.writeValue('A');

    delay(200);
  }
}
