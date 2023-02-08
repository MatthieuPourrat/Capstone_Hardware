#include <Arduino.h>
#include "MQ7.h"
#include "DHT22.h"
#include <iostream>
#include <Wire.h>
#include "MAX30105.h" //https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library
#include "heartRate.h"
#include "LoRa.h" //https://github.com/sandeepmistry/arduino-LoRa
#include <string.h>

//Pin definition for LoRa
#define ss 5
#define rst 25
#define dio0 33
//


MAX30105 particleSensor; //Object for the heart sensor


float ppmLoRa,tempLoRa, heartRateLoRa = 0.0; //Variables for the LoRa transmission


float heartRate()
{
  //98% of this code for the heartRate function is from: https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library

  unsigned long startRecording = micros();
  const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
  byte rates[RATE_SIZE]; //Array of heart rates
  byte rateSpot = 0;
  long lastBeat = 0; //Time at which the last beat occurred
  float beatsPerMinute;
  int beatAvg;

  while(micros() - startRecording < pow(10,7)) // run for 10 seconds.
  {
    long irValue = particleSensor.getIR();

    //We sensed a beat!
    if(irValue > 50000)
    {
      float delta = millis() - lastBeat;
      lastBeat = millis();

      beatsPerMinute = (60000 * (delta/1000)) / 100;

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
    
  /*
    Serial.print("IR=");
    Serial.print(irValue);
    Serial.print(", BPM=");
    Serial.print(beatsPerMinute);
    Serial.print(", Avg BPM=");
    Serial.print(beatAvg);
  */
    Serial.println("Processing Heart Beat...........................................");
    /*
    if (irValue < 50000)
      Serial.print(" No finger?");
    Serial.println();
    */
  }
  Serial.println("---------------Heart Rate--------------");
  Serial.println(beatsPerMinute);
  Serial.println("------------------------------------------");
  Serial.println();
  return beatsPerMinute;
}

void sendLoRa(int id, float temperaturLoRa, float ppm, float heartRate) //LoRa function to send the packets
{
  //Most of the code for this funtion is from: https://github.com/sandeepmistry/arduino-LoRa. 
  //Adapted for the use of this project and the microcontroller used.
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
  Serial.println(". End of Packet.");
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


  //Block 1: Heart Rate Startup //https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  //End of Block 1.

  delay(1000);
  //Block 2: LoRa startup from https://github.com/sandeepmistry/arduino-LoRa
  Serial.println("LoRa Starting");
  LoRa.setPins(ss,rst,dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //End of Block 2.
}

void loop() {

  MQ7 deviceMQ7; //New MQ& object
  DHT22 deviceDHT22; //New DHT22 object
  deviceMQ7.computeRs(); //Computer the surface resitance of the MQ7
  ppmLoRa = deviceMQ7.computePPM(); //Compute the PPM level, save value in ppmLoRa
  deviceMQ7.printMQ7(); //Print the results.
  delay(1000);
  deviceDHT22.readSensor(); //read the sensor and get the data
  deviceDHT22.computeHumidity(); //Compute RH
  tempLoRa = deviceDHT22.computeTemperature(); //Compute temperature and assign it to tempLoRa
  deviceDHT22.print(); //print
  delay(3000);
  heartRateLoRa = heartRate(); //Run the heart beat function and assign the value to hearRateLoRa
  sendLoRa(1,tempLoRa, ppmLoRa, heartRateLoRa); //Use all the values and send the packet through LoRa.
}
