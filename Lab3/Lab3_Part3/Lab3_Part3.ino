// RGB pins (should be capable of PWM)
#define RPIN 2
#define GPIN 5
#define BPIN 3
#define PB 18

/*
  Only certain colors constitute as "absolute", meaning that 
  the R, G, and B channels are at their minimum value (0) or
  their maximum value (255). Orange is an example of an
  intermediary color because the green channel rests at the 
  mid-point (128)

            R   G   B
  RED      255  0   0
  YELLOW   255 255  0
  GREEN     0  255  0
  CYAN      0  255 255
  BLUE      0   0  255
  MAGENTA  255  0  255
*/
#define MAX_VAL 255

enum colors
{
  RED = 0,
  YELLOW,
  GREEN,
  CYAN,
  BLUE,
  MAGENTA,
  TRANS  
};

// RGB888
struct color
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
} col;

struct repeat
{
  long var_delay;   // Variable delay in milliseconds between steps
  unsigned state;   // The current color state
} rep;

void setup() 
{
  Serial.begin(9600); 
  randomSeed(analogRead(0)); // Initialize random seed
  
  pinMode(RPIN, OUTPUT);
  pinMode(GPIN, OUTPUT);
  pinMode(BPIN, OUTPUT);
  pinMode(PB, INPUT);

  col = { 0 };
  rep = { 
    .var_delay = 5,
    .state = RED
  };
}

void loop() 
{ 
  /*
   * Check if push button is pressed
   * If so, go into "manual mode", then select
   * random colors for 10 cycles
   */
  if (digitalRead(PB) == HIGH)
  {
    for (int j = 0; j < 10; j++)
    {  
      delay(100); // We require a delay to prevent from the digitalRead check occurring too soon 
      while (true)
      {
        if (digitalRead(PB) == HIGH) break;  
      }  
      rand_col();
    }
    delay(100);

    // Reset structs
    col.r = col.g = col.b = 0;
    rep.state = RED;
  }
  
  analogWrite(RPIN, col.r);
  analogWrite(GPIN, col.g);
  analogWrite(BPIN, col.b);
  
  switch (rep.state)
  {
    case RED:
    {
      if (col.r == MAX_VAL)
      {
        rep.state = YELLOW;
      }
      else
      {
        col.r++;  
      }
      break;
    }  
    case YELLOW:
    {
      if (col.g == MAX_VAL)
      {
        rep.state = GREEN;
      }
      else
      {
        col.g++;  
      }
      break;
    }
    case GREEN:
    {
      if (col.r == 0)
      {
        rep.state = CYAN;
      }
      else
      {
        col.r--;  
      }
      break;
    }
    case CYAN:
    {
      if (col.b == 255)
      {
        rep.state = BLUE;
      }
      else
      {
        col.b++;  
      }
      break;
    }
    case BLUE:
    {
      if (col.g == 0)
      {
        rep.state = MAGENTA;
      }
      else
      {
        col.g--;  
      }
      break;
    }
    case MAGENTA:
    {
      if (col.r == 255)
      {
        rep.state = TRANS;
      }
      else
      {
        col.r++;  
      }
      break;
    }
    case TRANS:
    {
      if (col.b == 0)
      {
        rep.state = RED;
      }
      else
      {
        col.b--;  
      }
      break;
    }
  }
  
  delay(rep.var_delay);
}

void rand_col()
{
  col.r = random(256);
  col.g = random(256);
  col.b = random(256);

  analogWrite(RPIN, col.r);
  analogWrite(GPIN, col.g);
  analogWrite(BPIN, col.b);
}
