/**
  Name: Neil Kingdom
  Email: king0482@algonquinlive.com
  Expected Grade: 100%
  Known Issues: N/A
*/

#include <Bounce2.h>

#define NUM_DIGITS (10 + 1)
#define PB 11
#define SRCLR 15
#define SRCLK 16
#define RCLK 17
#define SER 18

static byte pbs_press = 1;
static byte round_num;
static byte rand_num;
static byte inp1, inp2;
static unsigned long prev_millis, curr_millis;

static const byte digits[NUM_DIGITS]
{
  0b00000011,  /* 0 */
  0b10011111,  /* 1 */
  0b00100101,  /* 2 */
  0b00001101,  /* 3 */
  0b10011001,  /* 4 */
  0b01001001,  /* 5 */
  0b01000001,  /* 6 */
  0b00011111,  /* 7 */
  0b00000001,  /* 8 */
  0b00011001,  /* 9 */
  0b11111110   /* DP */
};

static Bounce b = Bounce(); // Instantiate a bounce object

/*** Forward function declarations ***/
boolean are_equal(uint8_t, uint8_t) __attribute__((always_inline));
boolean are_equal(uint8_t inp1, uint8_t inp2) {
  return (inp1 == inp2);
}

boolean is_even(int) __attribute__((always_inline));
boolean is_even(int n) {
  return (n % 2 == 0);
}

void setup()
{
  Serial.begin(9600);
  // Initialize RNG seed (ensure pin 14 is not tied high or low)
  randomSeed(analogRead(0));

  /*** Configure pin modes ***/

  // Attach debouncer to push button at an interval of 20ms
  b.attach(PB, INPUT_PULLUP);
  b.interval(20);
  pinMode(SER, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLR, OUTPUT);

  /*** Output pin states ***/
  digitalWrite(PB, HIGH);
  // Clear shift register (active low)
  digitalWrite(SRCLR, LOW);
  digitalWrite(SRCLR, HIGH);

  Serial.println("Round #" + String(++round_num));
  prev_millis = millis();
}

void loop()
{
  /*** Output random digits to 7-segment display ***/
  if ((curr_millis = millis()) >= (prev_millis + 500))
  {
    prev_millis = curr_millis;
    rand_num = random(10);

    // Pull latch low when shifting serial data to shift register
    digitalWrite(RCLK, LOW);
    shiftOut(SER, SRCLK, LSBFIRST, digits[rand_num]);
    digitalWrite(RCLK, HIGH);
  }

  /*** Poll for push button action ***/
  b.update();
  if (b.fell())
  {
    Serial.println("PBS press # [" + String(pbs_press) + " | 2] ==> [" + String(rand_num) + "]");

    // Store input(s)
    if (!inp1) inp1 = rand_num;
    else inp2 = rand_num;

    if (!is_even((int)pbs_press))
    {
      delay(2000);
    }
    else
    {
      if (are_equal(inp1, inp2))
      {
        Serial.println("=> **MATCH**");
        // Blink effect
        for (int i = 0; i < 4; i++)
        {
          byte out = (!is_even(i)) ? digits[rand_num] : 0xFF; // 0xFF = blank
          digitalWrite(RCLK, LOW);
          shiftOut(SER, SRCLK, LSBFIRST, out);
          digitalWrite(RCLK, HIGH);
          delay(2000 / 4);
        }
      }
      else
      {
        Serial.println("=> NO Match... period.");
        digitalWrite(RCLK, LOW);
        shiftOut(SER, SRCLK, LSBFIRST, digits[(NUM_DIGITS - 1)]);
        digitalWrite(RCLK, HIGH);
        delay(2000);
      }

      inp1 = inp2 = 0; // Reset inputs
      Serial.println("Round #" + String(++round_num));
    }

    // Toggle between 1 and 2
    pbs_press ^= (0x01 | 0x02);
  }
}
