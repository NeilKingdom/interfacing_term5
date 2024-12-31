const int ledPin = 20;
const int pButton = 19;

void setup() 
{
  pinMode(ledPin, OUTPUT);
  pinMode(pButton, INPUT);
}

void loop() 
{
  while (digitalRead(pButton) == HIGH)
  {
    digitalWrite(ledPin, HIGH);  
  }
  digitalWrite(ledPin, LOW);  
}
