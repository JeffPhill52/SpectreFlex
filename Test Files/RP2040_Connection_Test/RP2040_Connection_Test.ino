//#include <WiFiNINA.h>

void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(4, INPUT);
}

void loop() {
  //Serial.println("Hello, World!");
  //delay(5000);

  // digitalWrite(3, HIGH);
  // //Serial.println("HIGH");
  // delay(500);
  // digitalWrite(3, LOW);
  // //Serial.println("LOW");
  // delay(500);

  if(!digitalRead(4))
  {
    digitalWrite(3, HIGH);
  }
  else
  {
    digitalWrite(3, LOW);
  }

}


