#define SRCLK 14 // Serial clock
#define RCLK  15 // Latch
#define SER   16 // Serial out
#define ONES  17 // Common cathode
#define TENS  18 // Common cathode

#define NUM_SEG 8
#define DIGITS  10

/*** 7-seg Display ***/
byte digits[DIGITS][NUM_SEG] = {
  /* A,  B,  C,  D,  E,  F,  G,  DP       OUTPUT */
   { 1,  1,  1,  1,  1,  1,  0,  0 }, // 11000000 0
   { 0,  1,  1,  0,  0,  0,  0,  0 }, // 11111001 1
   { 1,  1,  0,  1,  1,  0,  1,  0 }, // 10100100 2
   { 1,  1,  1,  1,  0,  0,  1,  0 }, // 10110000 3
   { 0,  1,  1,  0,  0,  1,  1,  0 }, // 10011001 4
   { 1,  0,  1,  1,  0,  1,  1,  0 }, // 10010010 5
   { 1,  0,  1,  1,  1,  1,  1,  0 }, // 10000010 6
   { 1,  1,  1,  0,  0,  0,  0,  0 }, // 11111000 7
   { 1,  1,  1,  1,  1,  1,  1,  0 }, // 10000000 8
   { 1,  1,  1,  1,  0,  1,  1,  0 }  // 10010000 9
};

static unsigned i;
static byte lo_byte; // The "1s" column
static byte hi_byte; // The "10s" column
volatile long curr_millis;
volatile long prev_millis;

void setup() 
{
  Serial.begin(9600);
  pinMode(SER, OUTPUT);  
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(ONES, OUTPUT);
  pinMode(TENS, OUTPUT);

  prev_millis = millis();
}

void loop() 
{   
  /*** Count from 0-99 on the 7-seg. display ***/
  
  // Sleep 200 (non-blocking)
  if (((curr_millis = millis()) - prev_millis) >= 200)
  {  
    prev_millis = curr_millis;
    lo_byte = hi_byte = 0x00;
    
    // Shift in the segment bits for the current number
    for (int j = 0; j < NUM_SEG; j++)
    {
      lo_byte |= ((!digits[i / 10][j]) << (8 - 1 - j));
      hi_byte |= ((!digits[i % 10][j]) << (8 - 1 - j));
    }

    i = (i < 99) ? (i + 1) : 0;
  }
    
  /*** Multiplex digits ***/

  // Write to "1s" column
  //digitalWrite(ONES, HIGH);
  //digitalWrite(TENS, LOW);
  digitalWrite(RCLK, LOW);
  shiftOut(SER, SRCLK, LSBFIRST, lo_byte);
  digitalWrite(RCLK, HIGH);

  // Write to "10s" column
  //digitalWrite(ONES, LOW);
  //digitalWrite(TENS, HIGH);
  digitalWrite(RCLK, LOW);
  shiftOut(SER, SRCLK, LSBFIRST, hi_byte);
  digitalWrite(RCLK, HIGH);
}
