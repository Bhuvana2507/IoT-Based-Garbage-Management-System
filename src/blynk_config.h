#pragma once
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>

// Blynk configuration
const char* BLYNK_AUTH_TOKEN = "YOUR_BLYNK_TOKEN";
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASS";

// Virtual pins
#define DISTANCE_PIN V0
#define STATUS_PIN V1
#define PERCENTAGE_PIN V2

// Blynk timer
SimpleTimer systemTimer;
