#include <Arduino.h>
#include <Wire.h>
#include "MAX30102.h"

MAX30102 device_max30102;
uint8_t answer =  0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  device_max30102.clearFIFO();
  device_max30102.enable_spo2();
}

void loop() {
  Serial.println(device_max30102.readRegisterFIFO());
  Serial.println();
  delay(1000);
}