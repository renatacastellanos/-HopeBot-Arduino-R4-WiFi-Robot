#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

#define TRIG_PIN 13
#define ECHO_PIN 12

void setupUltrasonic() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000UL); 
  if (duration == 0) return 999; 
  return duration / 58.0; 
}

#endif