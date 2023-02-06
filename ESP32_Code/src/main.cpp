#include <Arduino.h>
#include "MQ7.h"
#include "DHT22.h"
#include <iostream>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "LoRa.h"
#include <string.h>

//This is the code for the hardware.

#define ss 5
#define rst 25
#define dio0 33
MAX30105 particleSensor;

float tempLoRa, heartRateLoRa = 0.0;
double ppmLoRa = 0.0;

float heartRate()
{
  Serial.println("---------------Heart Rate--------------");
  const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
  byte rates[RATE_SIZE]; //Array of heart rates
  byte rateSpot = 0;
  long lastBeat = 0; //Time at which the last beat occurred
  unsigned long startRecording = micros();

  float beatsPerMinute;
  int beatAvg;

  while(micros() - startRecording < pow(10,7)) // run for 10 seconds.
  {
    long irValue = particleSensor.getIR();

    if (checkForBeat(irValue) == true)
    {
      //We sensed a beat!
      long delta = millis() - lastBeat;
      lastBeat = millis();

      beatsPerMinute = 60 / (delta / 1000.0);

      if (beatsPerMinute < 255 && beatsPerMinute > 20)
      {
        rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
        rateSpot %= RATE_SIZE; //Wrap variable

        //Take average of readings
        beatAvg = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      }
    }

    Serial.print("IR=");
    Serial.print(irValue);
    Serial.print(", BPM=");
    Serial.print(beatsPerMinute);
    Serial.print(", Avg BPM=");
    Serial.print(beatAvg);

    if (irValue < 50000)
      Serial.print(" No finger?");
  }

  Serial.println("------------------------------------------");
  Serial.println();
  return beatsPerMinute;
}

void sendLoRa(int id, float temperaturLoRa, double ppm, float heartRate)
{
  Serial.println("-------------------LoRa-------------------");
  Serial.print("Start of packet: ");
  Serial.print("ID: ");
  Serial.print(id);
  Serial.print(", ");
  Serial.print("Temperature: ");
  Serial.print(temperaturLoRa);
  Serial.print(", ");
  Serial.print("Carbon Monoxide: ");
  Serial.print(ppm);
  Serial.print(", ");
  Serial.print("Heart Rate: ");
  Serial.print(heartRate);
  Serial.print(". End of Packet.");
  Serial.println("------------------------------------------");

  // send packet
  LoRa.beginPacket();
  LoRa.print(id);
  LoRa.print("a");
  LoRa.print(temperaturLoRa);
  LoRa.print("b");
  LoRa.print(ppm);
  LoRa.print("c");
  LoRa.print(heartRate);
  LoRa.endPacket();

  Serial.println();
  delay(5000);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Program has started.");

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running

  delay(1000);

  Serial.println("LoRa Starting");
  LoRa.setPins(ss,rst,dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  MQ7 deviceMQ7;
  DHT22 deviceDHT22;
  deviceMQ7.computeRs();
  ppmLoRa = deviceMQ7.computePPM();
  deviceMQ7.printMQ7();
  delay(1000);
  deviceDHT22.readSensor();
  deviceDHT22.computeHumidity();
  tempLoRa = deviceDHT22.computeTemperature();
  deviceDHT22.print();
  delay(3000);
  heartRateLoRa = heartRate();
  sendLoRa(1,tempLoRa, ppmLoRa, heartRateLoRa);
}
