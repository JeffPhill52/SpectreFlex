

// Libraries for Bloothtooth Module
#include <ArduinoBLE.h>
#include <Arduino_LSM6DSOX.h>

// Bluetooth Initialization and Constants 
BLEService controlService("6F457C49-2D99-4334-B087-B7CD9E62D807");

//Gyro characteristic can be read and notified by remote device
BLECharacteristic testCharacteristic("6F457C49-2D99-4334-B087-B7CD9E62D808", BLERead | BLENotify, 20, 20);


// Flex sensor pins

const int FLEX1 = A0;     // Thumb
const int FLEX2 = A1;     // Index
const int FLEX3 = A2;     // Middle
const int FLEX4 = A3;     // Ring

const int FLEX1_THRESHOLD = 550;
const int FLEX2_THRESHOLD = 570;
const int FLEX3_THRESHOLD = 550;
const int FLEX4_THRESHOLD = 580;

const int GYRO_RANGE = 500;


char msgBuffer[32];

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

  BLE.setAdvertisedService(controlService);
  controlService.addCharacteristic(testCharacteristic);

  BLE.addService(controlService);

  testCharacteristic.writeValue((byte) 0xFF);
  
  BLE.advertise();

}

void setup() {

  Serial.begin(9600);
  Serial.println("Arduino Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
}

  startBLE();
}

byte BoolArrayToByte(bool boolArray[8]){
  byte result = 0;

  for(int i = 0; i < 8; ++i){
    if(boolArray[i])
      result = result | (i << i);
  }
  return result;
}

void floatFormatting(byte output[4] ,float x, float y, float z) {
  bool x_sign, y_sign, z_sign;
  int x_one, y_one, z_one, x_dec, y_dec, z_dec, out0, out1, out2, out3;
  bool bArr0[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  bool bArr1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  bool bArr2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  bool bArr3[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  x_one = (int) x;
  y_one = (int) y;
  z_one = (int) z;

  if  (x < 0)
    x_sign = true;
  else
    x_sign = false;
  if  (y < 0)
    y_sign = true;
  else
    y_sign = false;
  if  (z < 0)
    z_sign = true;
  else
    z_sign = false;

  bArr0[7] = x_sign;
  bArr1[7] = y_sign;
  bArr2[7] = z_sign;
  bArr0[6] = x_one;
  bArr1[6] = y_one;
  bArr2[6] = z_one;

  x_dec = abs((x - x_one) * 100);
  y_dec = abs((y - y_one) * 100);
  z_dec = abs((z - z_one) * 100);

  for (int i = 6; i > 0; --i){
    bArr0[i - 1] = bitRead(x_dec,i);
    bArr1[i - 1] = bitRead(y_dec,i);
    bArr2[i - 1] = bitRead(z_dec,i);
  }
  bArr3[0] = bitRead(x_dec,0);
  bArr3[1] = bitRead(y_dec,0);
  bArr3[2] = bitRead(z_dec,0);
  
  out0 = 0;
  out1 = 0;
  out2 = 0;
  out3 = 0;

  for (int i = 7; i >= 0; --i){
    out0 += bArr0[i]*pow(2, i);
    out1 += bArr1[i]*pow(2, i);
    out2 += bArr2[i]*pow(2, i);
    out3 += bArr3[i]*pow(2, i);
  }

  output[0] = byte(out0);
  output[1] = byte(out1);
  output[2] = byte(out2);
  output[3] = byte(out3);
}

void loop() { 
  BLEDevice central = BLE.central();

  float x_g, y_g, z_g, x_a, y_a, z_a;
  byte flex1_b, flex2_b, flex3_b, flex4_b, gyroX_b, gyroY_b, gyroZ_b, acclX_b, acclY_b, acclZ_b;
  byte acclFormat[4] = {(byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00};
  byte gyroFormat[4] = {(byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00};

  if (IMU.gyroscopeAvailable()){
    IMU.readGyroscope(x_g, y_g, z_g);
    if (x_g > GYRO_RANGE)
      x_g = GYRO_RANGE;
    else if (x_g < -GYRO_RANGE)
      x_g = -GYRO_RANGE;

    if (y_g > GYRO_RANGE)
      y_g = GYRO_RANGE;
    else if (y_g < -GYRO_RANGE)
      y_g = -GYRO_RANGE;

    if (z_g > GYRO_RANGE)
      z_g = GYRO_RANGE;
    else if (z_g < -GYRO_RANGE)
      z_g = -GYRO_RANGE;

    x_g = ( x_g / GYRO_RANGE ) * 1.99;
    y_g = ( y_g / GYRO_RANGE ) * 1.99;
    z_g = ( z_g / GYRO_RANGE ) * 1.99;

    floatFormatting(gyroFormat, x_g, y_g, z_g);

  }
  else
    x_g = 0.00, y_g = 0.00, z_g = 0.00;
  if (IMU.accelerationAvailable()){
    IMU.readAcceleration(x_a, y_a, z_a);
    floatFormatting(acclFormat, x_a, y_a, z_a);
    
  }
  else
    x_a = 0.00, y_a = 0.00, z_a = 1.00;
  
  gyroX_b = (byte) ((unsigned int) (x_g));
  gyroY_b = (byte) ((unsigned int) (y_g));
  gyroZ_b = (byte) ((unsigned int) (z_g));

  acclX_b = (byte) ((int) (x_a));
  acclY_b = (byte) ( (int) (y_a*125));
  acclZ_b = (byte) ((int) (z_a));

//  Serial.println("Gyro: ( " + String(x_g) + ", " + String(y_g) + ", " + String(z_g) + " )");
//  Serial.println("Accel: ( " + String(acclFormat[0]) + ", " + String(acclFormat[1]) + ", " + String(acclFormat[2]) + ", " + String(acclFormat[3]) + " )");
//  Serial.println("Accel: ( " + String(x_a) + ", " + String(y_a) + ", " + String(z_a) + " )");
//  Serial.println(String("Flex Sensors: \n\t") + String("FLEX 1: ") + String(analogRead(FLEX1)) + String("\n\tFLEX 2: ") + String(analogRead(FLEX2)) + String("\n\tFLEX 3: ") + String(analogRead(FLEX3)) + String("\n\tFLEX 4: ") + String(analogRead(FLEX4)));
  
  
  if(analogRead(FLEX1) < FLEX1_THRESHOLD) {
    flex1_b = (byte) 0x01;
  }
  else{
    flex1_b = (byte) 0x00;
  }
  if(analogRead(FLEX2) < FLEX2_THRESHOLD) {
    flex2_b = (byte) 0x01;
  }
  else{
    flex2_b = (byte) 0x00;
  }
  if(analogRead(FLEX3) < FLEX3_THRESHOLD) {
    flex3_b = (byte) 0x01;
  }
  else{
    flex3_b = (byte) 0x00;
  }
  if(analogRead(FLEX4) < FLEX4_THRESHOLD) {
    flex4_b = (byte) 0x01;
  }
  else{
    flex4_b = (byte) 0x00;
  }

  byte maxByte[20] = {flex1_b, flex2_b, flex3_b, flex4_b, acclFormat[0], acclFormat[1], acclFormat[2], acclFormat[3], gyroFormat[0], gyroFormat[1], gyroFormat[2], gyroFormat[3], (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF};

  if (central.connected())
  {
    testCharacteristic.writeValue(maxByte, 20);


  }
  delay(5);
}
