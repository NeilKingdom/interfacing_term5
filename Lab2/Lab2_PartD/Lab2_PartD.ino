const int ledPin = 20;

void setup() 
{
  pinMode(ledPin, OUTPUT);
}

void loop() 
{
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(1000);                  // wait for half a second
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(1000);                  // wait for half a second
}
