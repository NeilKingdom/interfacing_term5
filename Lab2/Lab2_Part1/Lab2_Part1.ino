// Lab 2
// Name: Neil Kingdom
// Student Number: 040967309
// Professor: Bhuwan Pharasi
// Date: 07/16/2022
//
// This program flashes the onboard LED of the Teenyduino every half a second

const int ledPin = 13;

void setup() 
{
  pinMode(ledPin, OUTPUT);
}

void loop() 
{
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(500);                   // wait for half a second
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(500);                   // wait for half a second
}
