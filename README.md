-HopeBot-Arduino-R4-WiFi-Robot

  A smart autonomous robot powered by the Arduino UNO R4 WiFi. It creates its own WiFi Hotspot to display a live dashboard on your phone or   computer, showing GPS location, obstacle distance, and PIR motion security alerts.

📂 Project Structure / Explanation of Files

  Main.ino: The master file that runs the robot. It coordinates the sensors, motors, and WiFi.

  gps_server.h: This file handles the complex WiFi logic. It creates the "hopebot" Hotspot, runs the Web Server for the dashboard, and       decodes the raw GPS data.

  webpage.h: Stores the actual HTML and JavaScript code for the dashboard interface you see in your browser.

  motors.h: Contains the specific driver logic to make the motors move forward, backward, and turn.

  ultrasonic.h: handles the math for converting sound waves (echo) into distance (centimeters) for obstacle avoidance.

  pir.h: Simply reads the digital signal from the motion sensor to trigger security alarms.

  buzzer.h: Generates different sound frequencies for startup beeps vs. security alarms.

  servo_head.h: Controls the servo motor to "look" left and right during autonomous navigation.

🔌 Pinout & Wiring Diagram

Component

Arduino Pin

Notes

Left Motor

5, 7

Pin 5 (Speed/PWM), Pin 7 (Direction)

Right Motor

6, 8

Pin 6 (Speed/PWM), Pin 8 (Direction)

Motor Standby

3

Must be HIGH to enable movement

Ultrasonic Trig

13

Sends the sound pulse

Ultrasonic Echo

12

Receives the sound pulse

PIR Sensor

11

High = Motion Detected

Servo Motor

10

Rotates the head

Buzzer

16 (A2)

Positive leg to 16, Negative to GND

GPS TX

14

Connects to Arduino Pin 14 (SoftwareSerial RX)

GPS RX

15

Connects to Arduino Pin 15 (SoftwareSerial TX)

Mode Button

2

Connects Pin 2 to GND when pressed

📚 Required Libraries

To compile this code, you must install these libraries in Arduino IDE:

TinyGPSPlus (for decoding GPS coordinates)

WiFiS3 (Built-in for Arduino R4)

SoftwareSerial (Built-in)

Servo (Built-in)

🚀 How to Operate

Power On: Switch the robot on.

Wait: Give it 3-5 seconds to initialize the WiFi and Sensors.

Connect: On your device, join the WiFi Network:

SSID: hopebot

Password: 24042404

Dashboard: Navigate to http://192.168.4.1 in your web browser.

Start: Press the physical button on Pin 2. The robot will beep and begin autonomous roaming.
