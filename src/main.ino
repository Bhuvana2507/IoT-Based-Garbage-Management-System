#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include "garbage_sensor.h"
#include "gsm_comms.h"
#include "blynk_config.h"

// System initialization
void setup() {
  Serial.begin(9600);
  initSensors();
  initGSM();
  Blynk.begin(BLYNK_AUTH_TOKEN, espSerial, 9600, WIFI_SSID, WIFI_PASSWORD);
  systemTimer.setInterval(SENSOR_READ_INTERVAL, checkBinLevel);
}

// Main loop
void loop() {
  Blynk.run();
  systemTimer.run();
}
