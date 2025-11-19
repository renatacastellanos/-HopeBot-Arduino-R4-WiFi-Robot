#ifndef GPS_SERVER_H
#define GPS_SERVER_H

#include <WiFiS3.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h> 
#include "webpage.h"

// === Settings ===
const char* ssid = "hopebot";    
const char* pass = "24042404";   

// === GPS Pins ===
// I SWAPPED THESE (15, 14) to see if it fixes the connection.
// Pin 15 is now RX (Listen), Pin 14 is now TX (Talk)
SoftwareSerial gpsSerial(15, 14); 

WiFiServer server(80);
TinyGPSPlus gps;

float currentLat = 0.0;
float currentLon = 0.0;
bool serverMotionDetected = false;
int serverObstacleDistance = 0;

// Debug timer
unsigned long lastDebugTime = 0;

void setupGPSandWiFi() {
  gpsSerial.begin(9600); 

  Serial.println("\n\n---------------------------------------------");
  Serial.println("🚀 STARTING WIFI & GPS...");
  
  // Create Access Point
  Serial.print("📡 Creating Hotspot: "); Serial.println(ssid);
  
  WiFi.beginAP(ssid, pass);
  delay(2000);

  if (WiFi.status() == WL_AP_LISTENING || WiFi.status() >= 0) {
    Serial.println("✅ HOTSPOT CREATED!");
    Serial.print("👉 IP: http://");
    Serial.println(WiFi.localIP());
    Serial.println("---------------------------------------------");
  } else {
    Serial.println("❌ Hotspot Failed");
  }
  
  server.begin();
}

void updateGPS() {
  // Feed GPS data to the library
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
    
    // *** DEBUG: UNCOMMENT THIS IF YOU STILL SEE NOTHING ***
    // Serial.write(c); // This prints the raw data coming from GPS
  }
  
  if (gps.location.isUpdated()) {
    currentLat = gps.location.lat();
    currentLon = gps.location.lng();
  }

  // --- DEBUGGING: Print Status to Serial Monitor every 3 seconds ---
  if (millis() - lastDebugTime > 3000) {
    lastDebugTime = millis();
    
    // 1. Check if we are getting ANY data from the wires
    if (gps.charsProcessed() < 10) {
      Serial.println("⚠️ NO DATA RECEIVED FROM GPS!");
      Serial.println("   - I swapped pins 14 & 15 in this code to try and fix it.");
      Serial.println("   - If this fails, please physically swap the wires.");
    } 
    // 2. Check if we have data but no fix (Unlikely since you said it's blinking)
    else if (gps.location.isValid() == false) {
      Serial.print("📡 Data Received (Chars: ");
      Serial.print(gps.charsProcessed());
      Serial.println(") -> Decoding Location...");
    } 
    // 3. We have a fix!
    else {
      Serial.print("✅ GPS LOCKED! Lat: ");
      Serial.print(currentLat, 6);
      Serial.println(" (Sending to Webpage...)");
    }
  }
}

void handleClient(bool motionStatus, int obstacleDist) {
  serverMotionDetected = motionStatus;
  serverObstacleDistance = obstacleDist;
  
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            if (request.indexOf("GET /data") >= 0) {
              client.print("{");
              client.print("\"lat\":"); client.print(currentLat, 6); client.print(",");
              client.print("\"lon\":"); client.print(currentLon, 6); client.print(",");
              client.print("\"motion\":"); client.print(serverMotionDetected ? "true" : "false"); client.print(",");
              client.print("\"obstacle\":"); client.print(serverObstacleDistance);
              client.print("}");
            } else {
              client.print(index_html);
            }
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }
}

#endif