#define SRCLK 14 // Serial clock
#define RCLK  15 // Latch
#define SER   16 // Serial out

#define SHF_LEFT  0
#define SHF_RIGHT 1

static byte led_byte;
static byte data_dir;

void setup() 
{
  Serial.begin(9600);
  pinMode(SER, OUTPUT);  
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);

  led_byte = 0x01;
  data_dir = SHF_RIGHT; // Starting direction will become inverted
}

void loop() 
{   
  // Ground latch while serial data is being written
  digitalWrite(RCLK, LOW);
  // Write serial data
  Serial.println(led_byte, BIN);
  shiftOut(SER, SRCLK, MSBFIRST, led_byte);
  digitalWrite(RCLK, HIGH);

  // Toggle data direction if at either end
  if (led_byte & 0x80 || led_byte & 0x01)
    data_dir ^= SHF_RIGHT;

  // Shift the LED "on" bit
  led_byte = (data_dir) ? (led_byte >> 1) : (led_byte << 1);

  delay(100);
}
