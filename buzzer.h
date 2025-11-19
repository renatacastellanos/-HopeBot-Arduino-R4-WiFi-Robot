#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

// === Buzzer Pin from your image ===
#define BUZZER_PIN 16 

void setupBuzzer() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void beep(int duration) {
  tone(BUZZER_PIN, 1000);
  delay(duration);
  noTone(BUZZER_PIN);
}

void alarmSound() {
  for(int i = 500; i < 1000; i+=100) {
    tone(BUZZER_PIN, i);
    delay(50);
  }
  noTone(BUZZER_PIN);
}

#endif