/*
  Arduino R4 WiFi Robot - COMPLETE WORKING VERSION
  - Mode 2: Waits for Button (Pin 2)
  - WiFi Hotspot: "hopebot" / "24042404"
  - GPS: Pins 14/15
  - PIR: Pin 11
  - Ultrasonic: Pins 13/12
*/

#include "motors.h"
#include "ultrasonic.h"
#include "buzzer.h"
#include "pir.h"
#include "gps_server.h"

// === Settings ===
#define OBSTACLE_DIST 25
#define ROAM_SPEED 100
#define TURN_SPEED 180
#define MODE_PIN 2

bool hasStarted = false;

void setup() {
  Serial.begin(9600);
  
  // Safety delay to let power stabilize and give you time to open Serial Monitor
  delay(3000);

  Serial.println("\n\n=============================================");
  Serial.println("🤖 SYSTEM STARTING UP...");
  Serial.println("=============================================");

  setupMotors();
  setupUltrasonic();
  setupBuzzer();
  setupPIR();
  
  // Start WiFi & GPS (This might take a few seconds)
  setupGPSandWiFi();
  
  pinMode(MODE_PIN, INPUT_PULLUP);

  Serial.println("✅ SYSTEM READY.");
  Serial.println("👉 WAITING FOR BUTTON PRESS (PIN 2)...");
  
  // Double beep to indicate ready
  beep(100); delay(100); beep(100);
}

void loop() {
  // 1. Update GPS & Sensors
  updateGPS();
  float distance = getDistance();
  bool motion = isMotionDetected();

  // 2. Handle Web Dashboard
  handleClient(motion, (int)distance);

  // 3. PIR Security Alert
  if (motion) {
    // Only print occasionally to avoid flooding serial
    static unsigned long lastPirTime = 0;
    if (millis() - lastPirTime > 1000) {
      Serial.println("ALARM: Motion Detected!");
      lastPirTime = millis();
      beep(50); 
    }
  }

  // 4. MODE 2 BUTTON CHECK
  if (!hasStarted) {
    if (digitalRead(MODE_PIN) == LOW) {
      Serial.println("✅ BUTTON PRESSED! MOTORS ENGAGED!");
      hasStarted = true;
      beep(500); 
      delay(500); 
    } else {
      // While waiting, we don't move motors, but WiFi/GPS still work
      return; 
    }
  }

  // 5. OBSTACLE AVOIDANCE LOGIC
  if (distance <= OBSTACLE_DIST && distance != 0) {
    Serial.println("Obstacle! Taking evasive action.");
    
    stopMotors();
    alarmSound(); 
    
    // Reverse
    moveBackward(ROAM_SPEED);
    delay(500);
    
    // Turn Right
    turnRight(TURN_SPEED);
    delay(600);
    
    stopMotors();
    delay(200);
    
  } else {
    // Path Clear -> Roam
    moveForward(ROAM_SPEED);
  }
  
  delay(10);
}
