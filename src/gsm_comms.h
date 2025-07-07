#pragma once
#include <SoftwareSerial.h>

// GSM configuration
const int GSM_RX_PIN = 8;
const int GSM_TX_PIN = 9;
SoftwareSerial gsmSerial(GSM_RX_PIN, GSM_TX_PIN);

// Initialize GSM module
void initGSM() {
  gsmSerial.begin(9600);
  delay(1000);
  gsmSerial.println("AT");
  delay(500);
  gsmSerial.println("AT+CMGF=1");
  delay(500);
}

// Send SMS alert
void sendSMSAlert(const String& message) {
  gsmSerial.print("AT+CMGS=\"");
  gsmSerial.print(ALERT_PHONE_NUMBER);
  gsmSerial.println("\"");
  delay(500);
  gsmSerial.print(message);
  delay(500);
  gsmSerial.write(26);
}
