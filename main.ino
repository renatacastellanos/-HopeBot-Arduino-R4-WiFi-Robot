/*
  Arduino R4 WiFi Robot - 45 DEGREE TURN VERSION
  - Mode 2: Waits for Button (Pin 2)
  - WiFi Hotspot: "hopebot" / "24042404"
  - Logic: Swerves 45 degrees to avoid, checks, then continues.
*/

#include "motors.h"
#include "ultrasonic.h"
#include "buzzer.h"
#include "pir.h"
#include "gps_server.h"
#include "servo_head.h" 

// === Settings ===
#define OBSTACLE_DIST 25
#define ROAM_SPEED 100
#define TURN_SPEED 180
#define MODE_PIN 2

// *** TUNE THIS NUMBER ***
// 400ms is approx 45 degrees. 
// 750ms is approx 90 degrees.
#define TURN_45_TIME 400 
#define TURN_EMERGENCY_TIME 800 // Used if the 45 deg turn is also blocked

bool hasStarted = false;

void setup() {
  Serial.begin(9600);
  delay(3000); // Startup safety delay

  Serial.println("\n\n=============================================");
  Serial.println("🤖 SYSTEM STARTING (45 DEGREE MODE)...");
  Serial.println("=============================================");

  setupMotors();
  setupUltrasonic();
  setupBuzzer();
  setupPIR();
  
  // 1. Setup Servo
  Serial.print("⚙️ Initializing Servo... ");
  setupServo();
  Serial.println("[OK]");
  delay(1000); 
  
  // 2. Setup WiFi
  Serial.println("📡 Starting WiFi & GPS...");
  setupGPSandWiFi();
  
  pinMode(MODE_PIN, INPUT_PULLUP);

  Serial.println("✅ SYSTEM READY.");
  Serial.println("👉 WAITING FOR BUTTON PRESS (PIN 2)...");
  
  beep(100); delay(100); beep(100);
}

void loop() {
  updateGPS();
  float distance = getDistance(); 
  bool motion = isMotionDetected();
  handleClient(motion, (int)distance);

  // PIR Alert
  if (motion) {
    static unsigned long lastPirTime = 0;
    if (millis() - lastPirTime > 1000) {
      Serial.println("ALARM: Motion Detected!");
      lastPirTime = millis();
      beep(50); 
    }
  }

  // Button Start Check
  if (!hasStarted) {
    if (digitalRead(MODE_PIN) == LOW) {
      Serial.println("✅ BUTTON PRESSED! GO!");
      hasStarted = true;
      beep(500); delay(500); 
    } else {
      return; 
    }
  }

  // === SMART OBSTACLE AVOIDANCE ===
  if (distance <= OBSTACLE_DIST && distance != 0) {
    Serial.println("Obstacle! Scanning...");
    
    stopMotors();
    alarmSound(); 
    
    // 1. Back up to make space
    moveBackward(ROAM_SPEED);
    delay(500);
    stopMotors();
    
    // 2. Scan Environment
    lookRight();
    delay(600); 
    int distRight = getDistance();
    delay(200);

    lookLeft();
    delay(800); 
    int distLeft = getDistance();
    delay(200);

    lookForward();
    delay(400);

    // 3. DECISION LOGIC (45 Degree Turns)
    
    if (distLeft < distRight) {
      // Object is NEARER on the Left -> Swerve RIGHT (45 deg)
      Serial.println("👉 Left blocked. Bearing RIGHT (45 deg).");
      turnRight(TURN_SPEED);
      delay(TURN_45_TIME);
      stopMotors();
      
      // *** DOUBLE CHECK ***
      delay(300);
      if (getDistance() <= OBSTACLE_DIST) {
        Serial.println("🚫 Still blocked! Doing Hard Turn.");
        moveBackward(ROAM_SPEED); delay(300);
        turnRight(TURN_SPEED);    delay(TURN_EMERGENCY_TIME); 
      }
    } 
    else if (distRight < distLeft) {
      // Object is NEARER on the Right -> Swerve LEFT (45 deg)
      Serial.println("👈 Right blocked. Bearing LEFT (45 deg).");
      turnLeft(TURN_SPEED);
      delay(TURN_45_TIME); 
      stopMotors();
      
      // *** DOUBLE CHECK ***
      delay(300);
      if (getDistance() <= OBSTACLE_DIST) {
        Serial.println("🚫 Still blocked! Doing Hard Turn.");
        moveBackward(ROAM_SPEED); delay(300);
        turnLeft(TURN_SPEED);     delay(TURN_EMERGENCY_TIME); 
      }
    } 
    else {
      // Equal -> Default Right Swerve
      Serial.println("🤷 Both equal. Bearing Right.");
      turnRight(TURN_SPEED);
      delay(TURN_45_TIME);
      stopMotors();
    }
    
    stopMotors();
    delay(200);
    
  } else {
    moveForward(ROAM_SPEED);
  }
  
  delay(10);
}
