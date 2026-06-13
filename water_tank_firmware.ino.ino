/*************************************************************
  Smart Water Tank Management System via Blynk IoT
  Processor: ESP32
  Sensors: HC-SR04 Ultrasonic Sensor
  Actuators: 5V Relay Module (Water Pump Control)
 *************************************************************/

/* Fill in credentials from your Blynk Device Info Dashboard */
#define BLYNK_TEMPLATE_ID   "TMPLxxxxxx"
#define BLYNK_TEMPLATE_NAME "WaterTankMonitoring"
#define BLYNK_AUTH_TOKEN    "YourAuthTokenHere"

/* Include Core Libraries */
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

/* WiFi Credentials */
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Your_WiFi_Name";
char pass[] = "Your_WiFi_Password";

/* Hardware Pin Configurations */
#define TRIG_PIN 5      // ESP32 Pin D5 connected to Ultrasonic Trigger
#define ECHO_PIN 18     // ESP32 Pin D18 connected to Ultrasonic Echo
#define RELAY_PIN 19    // ESP32 Pin D19 connected to Relay Module

/* Tank Physical Dimensions (in centimeters) */
const int TANK_DEPTH = 50;       // Total height of the tank from sensor to bottom
const int PUMP_START_LEVEL = 15; // Start pump when water level drops below 15%
const int PUMP_STOP_LEVEL = 95;  // Shut off pump when water reaches 95%

BlynkTimer timer;

// Function to read ultrasonic sensor and process tank logic
void checkWaterLevel() {
  long duration;
  int distance;
  int waterLevelPercentage;

  // Clear the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Set the trigger pin HIGH for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the echo pin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculating the distance (Speed of sound wave divided by 2)
  distance = duration * 0.034 / 2;
  
  // Calculate water height and percentage
  int waterHeight = TANK_DEPTH - distance;
  if (waterHeight < 0) waterHeight = 0;
  
  waterLevelPercentage = (waterHeight * 100) / TANK_DEPTH;
  if (waterLevelPercentage > 100) waterLevelPercentage = 100;
  if (waterLevelPercentage < 0) waterLevelPercentage = 0;

  // Output logs to Serial Monitor
  Serial.print("Distance to Water: ");
  Serial.print(distance);
  Serial.print("cm | Water Level: ");
  Serial.print(waterLevelPercentage);
  Serial.println("%");

  // Send data streams to Blynk App Virtual Pins
  Blynk.virtualWrite(V1, waterLevelPercentage); // Virtual Gauge / Level Widget
  Blynk.virtualWrite(V2, distance);             // Raw distance metrics

  // Automatic Automation Logic (Fill Tank)
  if (waterLevelPercentage <= PUMP_START_LEVEL) {
    digitalWrite(RELAY_PIN, HIGH);        // Turn on Relay (Pump ON)
    Blynk.virtualWrite(V3, "PUMP ACTIVE"); // Update status string on app
    Serial.println("Warning: Low Water. Pump Activated.");
  } 
  else if (waterLevelPercentage >= PUMP_STOP_LEVEL) {
    digitalWrite(RELAY_PIN, LOW);          // Turn off Relay (Pump OFF)
    Blynk.virtualWrite(V3, "TANK FULL");   // Update status string on app
    Serial.println("Notification: Tank Full. Pump Deactivated.");
  }
}

void setup() {
  // Initialize debugging serial port
  Serial.begin(115200);

  // Configure pin directions
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Ensure pump starts turned off (Assuming Active-High Relay)
  digitalWrite(RELAY_PIN, LOW);

  // Connect to Blynk cloud network
  Blynk.begin(auth, ssid, pass);

  // Setup a timer function to read the sensor every 2 seconds
  timer.setInterval(2000L, checkWaterLevel);
}

void loop() {
  Blynk.run();
  timer.run();
}