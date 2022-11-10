#include <CapacitiveSensor.h>
#include "pitches.h"
#include "limits.h"

#undef  ULONG_MAX
#define ULONG_MAX (LONG_MAX * 2UL + 1UL)
#define OUT 20
#define TX  21
#define RX  22
#define LED 13

static long val, max_val, min_val = ULONG_MAX;
CapacitiveSensor sensor = CapacitiveSensor(TX, RX);

void setup()
{
  Serial.begin(9600);

  pinMode(OUT, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(TX, OUTPUT);
  pinMode(RX, INPUT);

  // Get min and max readings
  digitalWrite(LED, HIGH);
  while (millis() < 5000)
  {
    val = analogRead(RX);
    if (val < min_val)
      min_val = val;
    if (val > max_val)
      max_val = val;
  }
  digitalWrite(LED, LOW);
}

void loop()
{
  // Buzzer
  //  if ((curr_millis = millis()) >= prev_millis + 1000)
  //  {
  //    prev_millis = curr_millis;
  //    res = analogRead(ANLG_READ);
  //    Serial.println(res, DEC);
  //    noTone(BUZZER);
  //  }

  //  if      (res >= 1000)
  //    tone(BUZZER, NOTE_C1);
  //  else if (res >= 875)
  //    tone(BUZZER, NOTE_C2);
  //  else if (res >= 750)
  //    tone(BUZZER, NOTE_C3);
  //  else if (res >= 625)
  //    tone(BUZZER, NOTE_C4);
  //  else if (res >= 500)
  //    tone(BUZZER, NOTE_C5);
  //  else if (res >= 375)
  //    tone(BUZZER, NOTE_C6);
  //  else if (res >= 150)
  //    tone(BUZZER, NOTE_C7);
  //  else if (res >= 0)
  //    tone(BUZZER, NOTE_C8);

  // LED
  val = sensor.capacitiveSensor(30); // Avg. of 30 values
  Serial.println(val, DEC);

  // Map min and max to 8-bit range
  map(val, min_val, max_val, 0, 255);
  analogWrite(OUT, val);

  delay(10);
}
