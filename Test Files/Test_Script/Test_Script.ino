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

// Flex sensor pins
const int FLEX1 = A0;     // Thumb
const int FLEX2 = A1;     // Index
const int FLEX3 = A2;     // Middle
const int FLEX4 = A3;     // Ring

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino Started");

  // output LEDs
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  // input tactile buttons
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);

    /* 
   *  Bluetooth Setup
   */
  BTserial.begin(9600);

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

}

void loop() {
  // put your main code here, to run repeatedly:

  // for(int i = 0; i < 5; i++)
  // {
  //   if(i == 2)
  //   {
  //     digitalWrite(3, HIGH);  
  //     delay(1000);
  //     digitalWrite(3, LOW);
  //   }
  //   else
  //   {
  //     digitalWrite(i + 4, HIGH);  
  //     delay(1000);
  //     digitalWrite(i + 4, LOW);
  //   }
  // }

  // /* Get new sensor events with the readings */
  // sensors_event_t a, g, temp;
  // mpu.getEvent(&a, &g, &temp);

  // //Serial.println("Accel: " + String(a.acceleration.x) + " " + String(a.acceleration.y) + " " + String(a.acceleration.z));
  // Serial.println("Gyro: " + String(g.gyro.x) + " " + String(g.gyro.y) + " " + String(g.gyro.z));
  // delay(100);

  if(digitalRead(9) == HIGH)
  {
    digitalWrite(8, HIGH);
  }
  else
  {
    digitalWrite(8, LOW);
  }
  if(digitalRead(10) == HIGH)
  {
    digitalWrite(7, HIGH);
  }
  else
  {
    digitalWrite(7, LOW);
  }
  if(digitalRead(11) == HIGH)
  {
    digitalWrite(6, HIGH);
  }
  else
  {
    digitalWrite(6, LOW);
  }
  if(digitalRead(12) == HIGH)
  {
    digitalWrite(5, HIGH);
  }
  else
  {
    digitalWrite(5, LOW);
  }

  Serial.print(String(analogRead(FLEX1)) + ", ");
  Serial.print(String(analogRead(FLEX2)) + ", ");
  Serial.print(String(analogRead(FLEX3)) + ", ");
  Serial.println(String(analogRead(FLEX4)));
  delay(250);





}
