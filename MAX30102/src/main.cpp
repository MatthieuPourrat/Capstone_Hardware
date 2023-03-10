#include <Arduino.h>
#include <Wire.h>
#include "MAX30102.h"

MAX30102 device_max30102;
uint8_t answer =  0;
float values[32];
int counter = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  device_max30102.clearFIFO();
  device_max30102.enable_spo2();
}

void loop() {
  if(counter != 31){
    values[counter] = device_max30102.computeHR();
    counter++;
  }
  else
  {
    counter = 0;
    for(int i = 0; i < 32; i++)
      Serial.println(values[i]);
    delay(5000);
  }
}