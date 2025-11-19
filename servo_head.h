#ifndef SERVO_HEAD_H
#define SERVO_HEAD_H

#include <Arduino.h>
#include <Servo.h>

// === Settings ===
#define SERVO_PIN 10
#define LOOK_CENTER 90
#define LOOK_RIGHT 10  // Adjust if looking too far/little
#define LOOK_LEFT 170  // Adjust if looking too far/little

Servo headServo;

void setupServo() {
  headServo.attach(SERVO_PIN);
  headServo.write(LOOK_CENTER); // Start looking forward
}

// Move head to center
void lookForward() {
  headServo.write(LOOK_CENTER);
}

// Move head right
void lookRight() {
  headServo.write(LOOK_RIGHT);
}

// Move head left
void lookLeft() {
  headServo.write(LOOK_LEFT);
}

#endif
