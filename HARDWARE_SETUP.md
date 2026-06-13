# Automated Water Tank Filling System - Architecture & Schematics

A lightweight, localized IoT infrastructure designed to monitor real-time water volume levels and automate fluid replenishment using an ESP32 micro-controller and the Blynk IoT dashboard app.

## 🛠️ Bill of Materials (BOM)
1. **Microcontroller:** NodeMCU ESP32 (Wi-Fi Enabled development board)
2. **Sensor:** HC-SR04 Ultrasonic Sensor Module
3. **Actuator:** 5V Single-Channel Optocoupler Relay Module
4. **Power Source:** 5V 1A DC Power Adapter (or USB Cable)
5. **Submersible Pump:** 5V/12V Mini DC Water Pump

---

## 🗺️ Visual Circuit Wiring Schematic

```text
       +-------------------------------------------------------------+
       |                                                             |
       |                   HC-SR04 ULTRASONIC SENSOR                 |
       |                     +-----------------+                     |
       |                     | VCC  Trig  Echo |                     |
       +---------------------+--+----+----+----+---------------------+
                                |    |    |
                               5V    GND  |
                                |    |    |
  +-----------------------------v----+----v--------------------------+
  |                            [VIN] [GND]                           |
  |                                                                  |
  |    NodeMCU ESP32          [G5]        [G18]        [G19]         |
  |    DEVELOPMENT BOARD        |           |            |           |
  +-----------------------------+-----------+------------+-----------+
                                |           |            |
                                +-----------+            |
                                  (Trigger)  (Echo)      |
                                                         |
                                                 (Relay Signal)
                                                         |
                                             +-----------v-----------+
                                             |  IN   GND    VCC      |
                                             |                       |
                                             |   5V RELAY MODULE     |
                                             |  [NO]   [COM]   [NC]  |
                                             +---+-------+-------+---+
                                                 |       |
                                                 |       |
                                           (POS +)       |
                                                 |       |
                                           +-----v----+  |
                                           |  DC MINI |  |
                                           |   WATER  |  |
                                           |   PUMP   |  |
                                           +-----+----+  |
                                                 |       |
                                              (NEG -)    |
                                                 |       |
                     [ 5V/12V DC POWER SUPPLY ]--+-------+


Component,Component Pin,ESP32 GPIO Pin,Power Rail
HC-SR04 Sensor,VCC,Vin (5V),External 5V / Board Vin
HC-SR04 Sensor,GND,GND,Ground Rail
HC-SR04 Sensor,Trig,GPIO 5 (D5),Signal Path
HC-SR04 Sensor,Echo,GPIO 18 (D18),Signal Path
5V Relay Module,VCC,Vin (5V),External 5V / Board Vin
5V Relay Module,GND,GND,Ground Rail
5V Relay Module,IN (Signal),GPIO 19 (D19),Signal Path
DC Water Pump,Positive (+),Relay NO (Normally Open),High-Current Supply

Blynk Cloud Software Datastream Configuration
To recreate the mobile interface dashboard, map the incoming hardware signals to the following Blynk Virtual Datastreams:

Virtual Pin V1 (Data Input): - Name: Water Level

Data Type: Integer (INT)

Range: Min: 0, Max: 100 (Units: %)

UI Widget: Level Gauge / Vertical Slider Component

Virtual Pin V2 (Data Input):

Name: Sensor Distance

Data Type: Integer (INT)

Range: Min: 0, Max: 500 (Units: cm)

UI Widget: Labeled Value Metric Box

Virtual Pin V3 (Status Output):

Name: Pump Operational State

Data Type: String (String)

UI Widget: Text Value Status Terminal
