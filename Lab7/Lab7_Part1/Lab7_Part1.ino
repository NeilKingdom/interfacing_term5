#include <CapacitiveSensor.h>
#include "pitches.h"
#include "math.h"

#define FLOAT_MAX (pow(2, (sizeof(float) * 8)))
#define BUZZER  9
#define LED_OUT 10
#define ONB_LED 13
#define TX      21
#define RX      22

static byte map_buzzer, map_led;
static float val, max_val, min_val = FLOAT_MAX;
CapacitiveSensor sensor = CapacitiveSensor(TX, RX);

void setup()
{
  Serial.begin(9600);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_OUT, OUTPUT);
  pinMode(ONB_LED, OUTPUT);
  pinMode(TX, OUTPUT);
  pinMode(RX, INPUT);

  // Get min and max readings
  digitalWrite(ONB_LED, HIGH);
  while (millis() < 5000)
  {
    val = sensor.capacitiveSensor(30);
    if (val < min_val)
      min_val = val;
    if (val > max_val)
      max_val = val;
  }
  digitalWrite(ONB_LED, LOW);

  // Make sure they are within proper bounds
  if (min_val < 0)
    min_val = 0;
  if (max_val > FLOAT_MAX)
    max_val = FLOAT_MAX;
}

void loop()
{
  // Get capacitance from tinfoil
  val = sensor.capacitiveSensor(30); // Avg. of 30 values

  // Buzzer
  noTone(BUZZER);
  // Map min and max to 8-bit range
  map_buzzer = (byte)map(val, min_val, max_val, NOTE_C1, NOTE_C8);
  tone(BUZZER, map_buzzer);

  // LED
  analogWrite(LED_OUT, map_led);
  map_led = (byte)map(val, min_val, max_val, 0, 255);
  Serial.println("Mapped value = " + String(map_led));

  delay(10);
}
