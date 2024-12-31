#include "IntervalTimer.h"

#define SRCLK 14 // Serial clock
#define RCLK  15 // Latch
#define SER   16 // Serial out

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

static byte lo_byte; // The "1s" column
static byte hi_byte; // The "10s" column
static volatile int i;

IntervalTimer timer;

void resetTimer()
{
  i = 0;  
}

void setup() 
{
  Serial.begin(9600);
  pinMode(SER, OUTPUT);  
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
}

void loop() 
{   
  // Clear interrupt flag (cli)
  noInterrupts();
  // Enable interrupts
  interrupts();
  
  /*
    Delay between increment is 200ms * 100 iterations
    This would be 20,000,000 microseconds
  */
  timer.begin(resetTimer, 200 * 100 * 1000);
  
  while (true)
  {  
    lo_byte = hi_byte = 0;
    
    // Shift in the segment bits for the current number
    for (int j = 0; j < NUM_SEG; j++)
    {
      lo_byte |= ((!digits[i % 10][j]) << (8 - 1 - j));
      hi_byte |= ((!digits[i / 10][j]) << (8 - 1 - j));
    }

    digitalWrite(RCLK, LOW);
    shiftOut(SER, SRCLK, LSBFIRST, lo_byte);
    shiftOut(SER, SRCLK, LSBFIRST, hi_byte);
    digitalWrite(RCLK, HIGH);

    delay(200);
    i++;
  }
}
