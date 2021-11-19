/*
 * Test hardware connected to glove
 * 
 * 
 * 
 */


// Flex sensor pins
const int FLEX1 = A0;     // Thumb
const int FLEX2 = A1;     // Index
const int FLEX3 = A2;     // Middle
const int FLEX4 = A3;     // Ring

// LEDs
// PLED connected to 5V
const int BLED = 3;

// Gyro Calibration Switch
const int zero_switch = 4;  // active low (low means button is pressed)

// variables
unsigned long flash_time = millis();
bool message_sent = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino Started");

  // output LEDs
  pinMode(BLED, OUTPUT);
  digitalWrite(BLED, HIGH);

  // input tactile buttons
  pinMode(zero_switch, INPUT);
}

void loop() {

  // test BLED
  bool led_on = millis() - flash_time >= 1000;
  bool reset_led = millis() - flash_time >= 2000;
  // if 1 second has passed and light is currently off
  if (led_on)
  {
    digitalWrite(BLED, HIGH);
    if (!message_sent)
    {
      //Serial.println("BLED on");
      message_sent = true;
    }
  }
  // if less than 1 second and light is on
  if (reset_led)
  {
    digitalWrite(BLED, LOW);
    flash_time = millis();
    message_sent = false;
  }

  Serial.print(analogRead(FLEX1));
  Serial.print("\t");
  Serial.print(analogRead(FLEX2));
  Serial.print("\t");
  Serial.print(analogRead(FLEX3));
  Serial.print("\t");
  Serial.print(analogRead(FLEX4));
  Serial.println();


//  Serial.print(digitalRead(zero_switch));
//  Serial.print("\t");
//  Serial.println(digitalRead(BLED));
  

  delay(10);

}
