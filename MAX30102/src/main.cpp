#include <Arduino.h>
#include <Wire.h>
#include "MAX30102.h"
#include "SoftwareSerial.h"

MAX30102 device_max30102;
int answer =  0;
float values[32];
int counter = 0;
float start, sum = 0.00;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  device_max30102.clearFIFO();
  device_max30102.enable_spo2();
}

void loop() {
  Serial.write(answer);
  answer = device_max30102.HR();
}