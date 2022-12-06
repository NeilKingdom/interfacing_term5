/**
  Name: Neil Kingdom
  Email: king0482@algonquinlive.com
  Expected Grade: 100%
  Known Issues: N/A
*/

#define MAX_TEMP 50
#define MIN_TEMP (-20)
#define NUM_DIGITS (10 + 1)

#define THERM 14
#define SRCLR 15
#define SRCLK 16
#define RCLK 17
#define SER 18

static int a0;
static float celsius;
static volatile uint8_t intr_flag; /* 0 = normal, 1 = low, 2 = high */
static long prev_millis, curr_millis;

static const byte nhl[NUM_DIGITS]
{
  0b11010101,  /* n --> Represents "Normal" Temperature */
  0b10010000,  /* H --> Represents "High" Temperature */
  0b11100010,  /* L --> Represents "Low" Temperature */
  0b11111110   /* DP */
};

IntervalTimer inj_lo_timer;
IntervalTimer inj_hi_timer;

void inject_low()
{
  a0 = -80;
  intr_flag = 1;
}

void inject_high()
{
  a0 = 800;
  intr_flag = 2;
}

float calc_celsius(int a0)
{
  if (a0 <= 289)
    celsius = 5 + (a0 - 289) / 9.82;
  if (a0 > 289 && a0 <= 342)
    celsius = 10 + (a0 - 342) / 10.60;
  if (a0 > 342 && a0 <= 398)
    celsius = 15 + (a0 - 398) / 11.12;
  if (a0 > 398 && a0 <= 455)
    celsius = 20 + (a0 - 455) / 11.36;
  if (a0 > 455 && a0 <= 512)
    celsius = 25 + (a0 - 512) / 11.32;
  if (a0 > 512 && a0 <= 566)
    celsius = 30 + (a0 - 566) / 11.00;
  if (a0 > 566 && a0 <= 619)
    celsius = 35 + (a0 - 619) / 10.44;
  if (a0 > 619 && a0 <= 667)
    celsius = 40 + (a0 - 667) / 9.73;
  if (a0 > 667)
    celsius = 45 + (a0 - 712) / 8.90;  

  return celsius;
}

void setup()
{
  Serial.begin(9600);

  /*** Configure pin modes ***/
  pinMode(SER, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLR, OUTPUT);
  pinMode(THERM, INPUT);

  /*** Output pin states ***/
  // Clear shift register (active low)
  digitalWrite(SRCLR, LOW);
  digitalWrite(SRCLR, HIGH);

  /*** Configure timers ***/
  inj_lo_timer.begin(inject_low, 2 * 1000 * 1000);
  inj_hi_timer.begin(inject_high, 4 * 1000 * 1000);

  prev_millis = millis();
}

void loop()
{
  /*** Continously print temperature with delay ***/
  for (int i = 0; i < 5; i++)
  {
    // Acquire lock whilst doing R/W on a0
    noInterrupts();
    if (!intr_flag) a0 = analogRead(THERM);
    celsius = calc_celsius(a0);
  
    // Pull latch low when shifting serial data to shift register
    digitalWrite(RCLK, LOW);
    if (celsius < MIN_TEMP) 
      shiftOut(SER, SRCLK, LSBFIRST, nhl[2]);
    else if (celsius > MAX_TEMP) 
      shiftOut(SER, SRCLK, LSBFIRST, nhl[1]);
    else 
      shiftOut(SER, SRCLK, LSBFIRST, nhl[0]);
    // Unlatch shift reg.
    digitalWrite(RCLK, HIGH);
  
    Serial.println("Temperature: " + String(a0) + " (raw) " + String(celsius) + " Celsius");
    // Release lock
    interrupts();

    // Delay for a fraction of 2 seconds (denominator depends on how many polls we do in for loop)
    delay((2 * 1000) / 5);
  }

  /*** Go back to reading normal temps every 6 seconds ***/
  if ((curr_millis = millis()) >= (prev_millis + 6 * 1000))
  {
    prev_millis = curr_millis;  
    intr_flag = 0;
    inj_lo_timer.end();
    inj_hi_timer.end();
    inj_lo_timer.begin(inject_low, 2 * 1000 * 1000);
    inj_hi_timer.begin(inject_high, 4 * 1000 * 1000);
  }

  /* 
   * The placement of this is very intentional 
   * as it must go after the previous block
   */
  if (intr_flag == 1) 
    Serial.println("INJECT: LOW");
  else if (intr_flag == 2)
    Serial.println("INJECT: HIGH");
}
