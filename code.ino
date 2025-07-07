#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>

// =====================
// Hardware Configuration
// =====================
#define ULTRASONIC_TRIG_PIN 2     // HC-SR04 Trigger pin
#define ULTRASONIC_ECHO_PIN 3     // HC-SR04 Echo pin
#define ESP8266_RX_PIN 10         // ESP8266 RX (Arduino TX)
#define ESP8266_TX_PIN 11         // ESP8266 TX (Arduino RX)
#define GSM_RX_PIN 8              // GSM RX (Arduino TX)
#define GSM_TX_PIN 9              // GSM TX (Arduino RX)

// =====================
// System Configuration
// =====================
const int FULL_DISTANCE_THRESHOLD = 10;   // Distance in cm when bin is full (10cm)
const int EMPTY_DISTANCE_THRESHOLD = 30;  // Distance in cm when bin is empty (30cm)
const int SENSOR_READ_INTERVAL = 5000;    // Read sensor every 5 seconds
const int GSM_POWER_ON_DELAY = 1000;      // Delay for GSM initialization
const int SMS_SEND_DELAY = 1000;          // Delay between SMS commands

// =====================
// Network Configuration
// =====================
const char* BLYNK_AUTH_TOKEN = "Your_Blynk_Auth_Token";
const char* WIFI_SSID = "Your_WiFi_SSID";
const char* WIFI_PASSWORD = "Your_WiFi_Password";

// =====================
// Alert Configuration
// =====================
const String ALERT_PHONE_NUMBER = "+919876543210"; // With country code
const String SMS_ALERT_MESSAGE = "ALERT: Garbage bin is FULL! Immediate attention required.";
const String BLYNK_ALERT_MESSAGE = "🚨 Garbage bin is FULL!";

// =====================
// Global Variables
// =====================
SoftwareSerial espSerial(ESP8266_RX_PIN, ESP8266_TX_PIN); // ESP8266 interface
SoftwareSerial gsmSerial(GSM_RX_PIN, GSM_TX_PIN);         // GSM module interface

SimpleTimer systemTimer;         // System timer for periodic tasks
bool binFullStatus = false;      // Current bin status
bool lastBinStatus = false;      // Previous bin status
bool alertSentStatus = false;    // Track if alert was sent

// =====================
// Function Prototypes
// =====================
void initializeSystem();
void initializeGSMModule();
void checkBinLevel();
int measureGarbageLevel();
void updateCloudStatus(int distance);
void sendSMSAlert();
void handleStatusChange(bool currentStatus);

// =====================
// Main Program
// =====================
void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println(F("\n===== IoT Garbage Management System ====="));
  Serial.println(F("Initializing system..."));
  
  // Initialize hardware components
  initializeSystem();
  
  // Initialize Blynk IoT platform
  Blynk.begin(BLYNK_AUTH_TOKEN, espSerial, 9600, WIFI_SSID, WIFI_PASSWORD);
  
  // Set up periodic bin checking
  systemTimer.setInterval(SENSOR_READ_INTERVAL, checkBinLevel);
  
  Serial.println(F("System initialization complete"));
  Serial.println(F("Monitoring garbage levels..."));
}

void loop() {
  // Handle Blynk communication
  Blynk.run();
  
  // Execute scheduled tasks
  systemTimer.run();
}

// =====================
// System Initialization
// =====================
void initializeSystem() {
  // Initialize ultrasonic sensor pins
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  
  // Initialize serial interfaces
  espSerial.begin(9600);
  gsmSerial.begin(9600);
  
  // Initialize GSM module
  delay(GSM_POWER_ON_DELAY);
  initializeGSMModule();
}

// =====================
// GSM Module Setup
// =====================
void initializeGSMModule() {
  Serial.println(F("Initializing GSM module..."));
  
  // Send basic AT commands to check module
  gsmSerial.println(F("AT"));
  delay(SMS_SEND_DELAY);
  
  // Set SMS text mode
  gsmSerial.println(F("AT+CMGF=1"));
  delay(SMS_SEND_DELAY);
  
  Serial.println(F("GSM module ready"));
}

// =====================
// Bin Level Monitoring
// =====================
void checkBinLevel() {
  // Measure current garbage level
  int currentDistance = measureGarbageLevel();
  
  // Determine bin status based on distance
  binFullStatus = (currentDistance <= FULL_DISTANCE_THRESHOLD);
  
  // Update cloud dashboard
  updateCloudStatus(currentDistance);
  
  // Handle status changes
  handleStatusChange(binFullStatus);
  
  // Store current status for next check
  lastBinStatus = binFullStatus;
}

// =====================
// Distance Measurement
// =====================
int measureGarbageLevel() {
  // Generate ultrasonic pulse
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  
  // Measure echo duration
  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  
  // Calculate distance in cm (speed of sound 340 m/s)
  int distance = duration * 0.034 / 2;
  
  // Log measurement
  Serial.print(F("Distance measured: "));
  Serial.print(distance);
  Serial.println(F(" cm"));
  
  // Calculate fill percentage
  int fillPercentage = map(
    distance, 
    EMPTY_DISTANCE_THRESHOLD, 
    FULL_DISTANCE_THRESHOLD, 
    0, 
    100
  );
  fillPercentage = constrain(fillPercentage, 0, 100);
  
  Serial.print(F("Bin fill level: "));
  Serial.print(fillPercentage);
  Serial.println(F("%"));
  
  return distance;
}

// =====================
// Cloud Communication
// =====================
void updateCloudStatus(int distance) {
  // Send raw distance value to Blynk (V0)
  Blynk.virtualWrite(V0, distance);
  
  // Send bin status (V1: 0=not full, 1=full)
  Blynk.virtualWrite(V1, binFullStatus ? 1 : 0);
  
  // Send fill percentage (V2)
  int fillPercentage = map(
    distance, 
    EMPTY_DISTANCE_THRESHOLD, 
    FULL_DISTANCE_THRESHOLD, 
    0, 
    100
  );
  fillPercentage = constrain(fillPercentage, 0, 100);
  Blynk.virtualWrite(V2, fillPercentage);
}

// =====================
// Status Change Handling
// =====================
void handleStatusChange(bool currentStatus) {
  // Check if status changed to full
  if (currentStatus && !lastBinStatus && !alertSentStatus) {
    Serial.println(F("Bin status changed to FULL"));
    
    // Send alerts
    sendSMSAlert();
    Blynk.notify(BLYNK_ALERT_MESSAGE);
    
    alertSentStatus = true;
  }
  
  // Check if bin was emptied
  if (!currentStatus && lastBinStatus) {
    Serial.println(F("Bin has been emptied"));
    alertSentStatus = false;
  }
}

// =====================
// SMS Alert System
// =====================
void sendSMSAlert() {
  Serial.println(F("Sending SMS alert..."));
  
  // Set recipient number
  gsmSerial.print(F("AT+CMGS=\""));
  gsmSerial.print(ALERT_PHONE_NUMBER);
  gsmSerial.println(F("\""));
  delay(SMS_SEND_DELAY);
  
  // Send alert message
  gsmSerial.print(SMS_ALERT_MESSAGE);
  delay(SMS_SEND_DELAY);
  
  // End message with CTRL+Z (ASCII 26)
  gsmSerial.write(26);
  delay(SMS_SEND_DELAY);
  
  Serial.println(F("SMS alert sent successfully"));
}
