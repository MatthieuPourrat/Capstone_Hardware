#include <Arduino.h>
#include "MQ7.h"
#include "DHT22.h"
#include <iostream>

void setup() {
  Serial.begin(9600);
  Serial.println("Program has started.");
}

void loop() {
  MQ7 deviceMQ7;
  DHT22 deviceDHT22;
  deviceMQ7.computeRs();
  deviceMQ7.computePPM();
  deviceMQ7.printMQ7();
  delay(1000);
  deviceDHT22.readSensor();
  deviceDHT22.computeHumidity();
  deviceDHT22.computeTemperature();
  deviceDHT22.print();
  delay(3000);
}