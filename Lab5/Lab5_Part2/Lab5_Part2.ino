#define SRCLK 14 // Serial clock
#define RCLK  15 // Latch
#define SER   16 // Serial out

static short led_short;

void setup() 
{
  Serial.begin(9600);
  pinMode(SER, OUTPUT);  
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
}

void loop() 
{   
  led_short = 0x00;
  
  for (int i = 0; i < 16; i++)
  {
    // Ground latch while serial data is being written
    digitalWrite(RCLK, LOW);
    // Write serial data
    Serial.println(led_short, BIN);
    shiftOut(SER, SRCLK, MSBFIRST, led_short >> 8);
    shiftOut(SER, SRCLK, MSBFIRST, led_short);
    digitalWrite(RCLK, HIGH);

    led_short |= 1 << i;

    delay(100);
  }

  for (int i = 0; i < 16; i++)
  {
    digitalWrite(RCLK, LOW);
    Serial.println(led_short, BIN);
    shiftOut(SER, SRCLK, MSBFIRST, led_short >> 8);
    shiftOut(SER, SRCLK, MSBFIRST, led_short);
    digitalWrite(RCLK, HIGH);

    /* 
     *  Note: There is a small overflow bug here which can be seen in 
     *  the serial console, however, it doesn't affect the output
     */
    led_short = 0xFFFF >> (i + 1);

    delay(100);
  }
}
