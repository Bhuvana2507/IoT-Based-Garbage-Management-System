#pragma once
#include <Arduino.h>

// Sensor configuration
const int TRIG_PIN = 2;
const int ECHO_PIN = 3;

// Initialize ultrasonic sensor
void initSensors() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

// Measure garbage level
float measureGarbageLevel() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2; // Return distance in cm
}
