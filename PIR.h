#ifndef PIR_H
#define PIR_H

#include <Arduino.h>

// === PIR Pin from your image ===
#define PIR_PIN 11 

void setupPIR() {
  pinMode(PIR_PIN, INPUT);
}

bool isMotionDetected() {
  return digitalRead(PIR_PIN) == HIGH;
}

#endif