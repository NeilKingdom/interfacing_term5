#define SRCLK 14 // Serial clock
#define RCLK  15 // Latch
#define SER   16 // Serial out

static unsigned short led_short;

void setup() 
{
  Serial.begin(9600);
  pinMode(SER, OUTPUT);  
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
}

void loop() 
{   
  led_short = 0;
  
  for (int i = 0; i < 16; i++)
  {
    Serial.println(led_short, BIN);
    digitalWrite(RCLK, LOW);
    shiftOut(SER, SRCLK, MSBFIRST, led_short >> 8);
    shiftOut(SER, SRCLK, MSBFIRST, led_short);
    digitalWrite(RCLK, HIGH);

    led_short |= (1 << i);

    delay(200);
  }

  for (int i = 0; i < 16; i++)
  {
    Serial.println(led_short, BIN);
    digitalWrite(RCLK, LOW);
    shiftOut(SER, SRCLK, MSBFIRST, led_short >> 8);
    shiftOut(SER, SRCLK, MSBFIRST, led_short);
    digitalWrite(RCLK, HIGH);
    
    led_short >>= 1;

    delay(200);
  }
}
