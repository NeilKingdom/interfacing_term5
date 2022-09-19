const int rpin = 2;
const int gpin = 5;
const int bpin = 3;

void setup() 
{
  pinMode(rpin, OUTPUT);
  pinMode(gpin, OUTPUT);
  pinMode(bpin, OUTPUT);
}

void loop() 
{
  digitalWrite(bpin, LOW);
  digitalWrite(rpin, HIGH);     // Blink RED LED
  delay(1000);                  
  digitalWrite(rpin, LOW);
  digitalWrite(gpin, HIGH);     // Blink GREEN LED
  delay(1000);                 
  digitalWrite(gpin, LOW);
  digitalWrite(bpin, HIGH);     // Blink BLUE LED
  delay(1000);
}
