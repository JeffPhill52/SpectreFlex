
// Libraries for Accelerometer
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Accelerometer Initialization and Constants
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino Started");

  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

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

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  //Serial.println("Accel: " + String(a.acceleration.x) + " " + String(a.acceleration.y) + " " + String(a.acceleration.z));
  Serial.println("Gyro: " + String(g.gyro.x) + " " + String(g.gyro.y) + " " + String(g.gyro.z));
  delay(100);

}
