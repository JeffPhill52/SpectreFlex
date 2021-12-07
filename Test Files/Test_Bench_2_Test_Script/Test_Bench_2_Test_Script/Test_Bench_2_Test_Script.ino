// Libraries for Bloothtooth Module
#include <SoftwareSerial.h>

// Libraries for Accelerometer
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Bluetooth Initialization and Constants
SoftwareSerial BTserial(2, 3); // RX | TX

// Accelerometer Initialization and Constants
Adafruit_MPU6050 mpu;

// Pin Declarations
const int BLUETOOTH_LED = 3;
const int LED1 = 4;
const int LED2 = 5;
const int LED3 = 6;

const int SW1 = 7;
const int SW2 = 8;

const int FLEX1 = 9;
const int FLEX2 = 10;
const int FLEX3 = 11;
const int FLEX4 = 12;



void setup() {
  // Initialize Serial Output
  Serial.begin(9600);
  Serial.println("Arduino Started");

  // Initialize input/output pins
  pinMode(BLUETOOTH_LED, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(FLEX1, INPUT);
  pinMode(FLEX2, INPUT);
  pinMode(FLEX3, INPUT);
  pinMode(FLEX4, INPUT);

  digitalWrite(BLUETOOTH_LED, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  Serial.println("Pins initialized");

  /* 
   *  Bluetooth Setup
   */
  BTserial.begin(9600);
  Serial.println("Bluetooth Module initialized, baud = 9600");

  /*
   * Accelerometer Setup
   */
  if (!mpu.begin()) 
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1) 
    {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("Accel/Gyro Module initialized");
  

}

void loop() {
  // put your main code here, to run repeatedly:


  if(digitalRead(FLEX1) == HIGH)
  {
    digitalWrite(LED1, HIGH);
  }
  else
  {
    digitalWrite(LED1, LOW);
  }
  if(digitalRead(FLEX2) == HIGH)
  {
    digitalWrite(LED2, HIGH);
  }
  else
  {
    digitalWrite(LED2, LOW);
  }
  if(digitalRead(FLEX3) == HIGH)
  {
    digitalWrite(LED3, HIGH);
  }
  else
  {
    digitalWrite(LED3, LOW);
  }


  Serial.print(String(digitalRead(FLEX1)) + ", ");
  Serial.print(String(digitalRead(FLEX2)) + ", ");
  Serial.print(String(digitalRead(FLEX3)) + ", ");
  Serial.println(String(digitalRead(FLEX4)));
  delay(250);

}
