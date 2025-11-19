#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

#define STBY 3
#define PWMA 5 
#define PWMB 6 
#define AIN1 7 
#define BIN1 8 

void setupMotors() {
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(BIN1, OUTPUT);
  digitalWrite(STBY, HIGH); 
}

void stopMotors() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

void moveForward(int speed) {
  digitalWrite(AIN1, HIGH); 
  digitalWrite(BIN1, HIGH); 
  analogWrite(PWMA, speed);   
  analogWrite(PWMB, speed);   
}

void moveBackward(int speed) {
  digitalWrite(AIN1, LOW);  
  digitalWrite(BIN1, LOW);  
  analogWrite(PWMA, speed);   
  analogWrite(PWMB, speed);
}

void turnRight(int speed) {
  digitalWrite(AIN1, HIGH); 
  digitalWrite(BIN1, LOW);  
  analogWrite(PWMA, speed);
  analogWrite(PWMB, speed);
}

void turnLeft(int speed) {
  digitalWrite(AIN1, LOW); 
  digitalWrite(BIN1, HIGH);  
  analogWrite(PWMA, speed);
  analogWrite(PWMB, speed);
}

#endif