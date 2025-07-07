# IoT-Based-Garbage-Management-System
> An intelligent waste management solution for smart cities
---

## Project Overview

This IoT-based system monitors garbage levels in public bins using **ultrasonic sensors** and provides real-time notifications to waste management authorities via **GSM SMS or cloud dashboard**. This helps in:
- Preventing overflow
- Reducing manual monitoring
- Enabling data-driven route optimization

It’s ideal for municipalities, smart campuses, and large communities.

---

## Key Features

- Real-time garbage level monitoring
- Cloud Dashboard via **Blynk IoT**
- SMS alerts for full bins via **GSM**
- Fill percentage display
- Historical data tracking
- Collection route planning (future scope)

---

## Hardware Components

| Component              | Quantity | Description                            |
|------------------------|----------|----------------------------------------|
| Arduino Uno            | 1        | Central controller                     |
| HC-SR04 Ultrasonic Sensor | 1     | Distance-based fill level detection    |
| ESP8266 Wi-Fi Module   | 1        | Sends data to cloud (optional)         |
| SIM800L GSM Module     | 1        | Sends SMS alerts                       |
| Breadboard             | 1        | Circuit testing and assembly           |
| Jumper Wires           | 20+      | Connections                            |
| Power Supply (12V/2A)  | 1        | Power for GSM, ESP, and Arduino        |
| Waterproof Enclosure   | 1        | Outdoor protection                     |

---

## Arduino Connections

### Ultrasonic Sensor to Arduino

| Sensor Pin | Arduino Pin |
|------------|-------------|
| VCC        | 5V          |
| GND        | GND         |
| TRIG       | D9          |
| ECHO       | D10         |

---

### ESP8266 Wi-Fi to Arduino (if using Blynk Dashboard)

| ESP8266 Pin | Arduino Pin         |
|-------------|---------------------|
| VCC         | 3.3V                |
| GND         | GND                 |
| TX          | D2 (Arduino RX)     |
| RX          | D3 (Arduino TX via voltage divider) |

> **Important:** ESP8266 RX pin must not get 5V directly. Use a voltage divider (1k + 2k) or logic level shifter.

---

### SIM800L GSM Module to Arduino (if using SMS alerts)

| GSM Pin | Arduino Pin |
|---------|-------------|
| VCC     | External 12V Supply (not from Arduino) |
| GND     | GND         |
| TX      | RX (D0)     |
| RX      | TX (D1)     |

> **Power Note:** SIM800L requires stable 12V 2A supply. Use a buck converter or battery pack.

---

## Arduino IDE Setup

### Download and Install Arduino IDE  
https://www.arduino.cc/en/software

### Install Required Libraries

- `BlynkSimpleEsp8266.h`
- `SoftwareSerial.h`

> Go to **Sketch > Include Library > Manage Libraries** to install.

---
## Blynk Cloud Setup (Optional - for Wi-Fi Monitoring)

This setup allows you to view garbage bin status remotely via your smartphone using the **Blynk IoT platform**.

### Step-by-Step Guide

1. **Install the Blynk IoT App**
   - Available on [Google Play Store](https://play.google.com/store/apps/details?id=cloud.blynk) and [Apple App Store](https://apps.apple.com/app/blynk-iot/id1558863276)

2. **Create a New Project**
   - Device: Select **ESP8266**
   - Connection: Wi-Fi
   - Project Name: `Smart Garbage Monitor`

3. **Receive Auth Token**
   - Blynk will send an **Auth Token** to your registered email. Save this for later.

4. **Add Widgets**
   - **Gauge** widget:
     - Set **Input Pin** as `Virtual Pin V0`
     - Name it "Garbage Level"
     - Range: 0–100 (%)
   - **Notification** widget:
     - Sends alerts when bin is full.

---

## Testing Procedure

Follow these step-by-step instructions to test your IoT-Based Garbage Management System:

### Step-by-Step:

1. **Place the ultrasonic sensor** inside the lid of a dustbin or container, facing downwards.
2. **Upload the complete code** to the Arduino using the Arduino IDE.
3. **Power the setup** using either:
   - USB (for Arduino + ESP8266 only)
   - 12V/2A adapter for Arduino + GSM module
4. **Open the Serial Monitor** in Arduino IDE (set baud rate to `9600`) to observe distance logs.
5. **Simulate bin filling:**
   - Gradually add objects into the bin.
   - Observe the sensor distance decrease.
   - See the calculated **fill percentage** increase on:
     - **Blynk dashboard** (if Wi-Fi is used)
     - **SMS notification** (if GSM is used)
6. **Verify behavior:**
   - If distance < threshold (e.g. 10 cm): 
     - Red LED turns ON
     - SMS/Blynk alert is sent
   - If distance > threshold:
     - Green LED turns ON
     - No alert sent

---

## Expected Results

| Condition                | Output                              |
|--------------------------|-------------------------------------|
| Bin less than 50% full   | Green LED ON, no alert              |
| Bin more than 80% full   | Red LED ON, alert via SMS/Blynk     |
| Normal fluctuation       | Stable reading every 5 seconds      |
| Wi-Fi connected          | Data shows in Blynk app             |
| GSM connected            | SMS sent to preset number           |

---

## Troubleshooting

| Issue                          | Solution                                                                 |
|-------------------------------|--------------------------------------------------------------------------|
| No sensor reading           | Check power supply, connections to TRIG/ECHO pins                        |
| Blynk not updating          | Recheck Auth Token, Wi-Fi SSID/password, and use 2.4GHz Wi-Fi            |
| SMS not sent                | Check SIM balance, signal, and use external 12V/2A adapter for SIM800L   |
| ESP8266 not connecting      | Avoid using Arduino's 3.3V — use external regulated 3.3V (via AMS1117)   |
| Sensor gives inconsistent values | Place sensor in stable, vertical orientation; no reflective material nearby  |
| App notification not received | Add Notification widget in Blynk + use `Blynk.logEvent()` in code         |

---

## Observations and Logs

Example Serial Monitor Output:

Garbage Distance: 25 cm
Fill Level: 40%
Status: Bin is NOT full

Garbage Distance: 9 cm
Fill Level: 90%
Status: Bin is FULL - Alert sent!

> Tip: You can log this data into a CSV or Google Sheet using Blynk Web dashboard or IoT platforms like ThingSpeak.

---

## Conclusion

The IoT-based garbage monitoring system allows for smart, automated garbage tracking with:

- Real-time monitoring
- Alert notifications (SMS or App)
- Optimized waste collection scheduling

It reduces manual intervention, avoids overflows, and helps create cleaner, smarter cities.

---

> 🌱 “Clean India, Smart India – Let’s build it with sensors and code.”


