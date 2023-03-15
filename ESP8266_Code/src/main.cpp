#include <Arduino.h>
#include "MQ7.h"
#include "DHT22.h"
#include <iostream>
#include <Wire.h>
#include "LoRa.h" //https://github.com/sandeepmistry/arduino-LoRa
#include <string.h>
#include "SoftwareSerial.h"

//Pin definition for LoRa
#define ss 5
#define rst 25
#define dio0 33
//

SoftwareSerial software_connection(3,1);

float ppmLoRa,tempLoRa, heartRateLoRa = 0.0; //Variables for the LoRa transmission

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

  software_connection.begin(9600);

  //Block 2: LoRa startup from https://github.com/sandeepmistry/arduino-LoRa
  // Serial.println("LoRa Starting");
  // LoRa.setPins(ss,rst,dio0);

  // if (!LoRa.begin(433E6)) {
  //   Serial.println("Starting LoRa failed!");
  //   while (1);
  // }
  //End of Block 2.
}

void loop() {

  //MQ7 deviceMQ7; //New MQ& object
  //DHT22 deviceDHT22; //New DHT22 object
  // deviceMQ7.computeRs(); //Computer the surface resitance of the MQ7
  // ppmLoRa = deviceMQ7.computePPM(); //Compute the PPM level, save value in ppmLoRa
  // deviceMQ7.printMQ7(); //Print the results.
  // delay(1000);
  // deviceDHT22.readSensor(); //read the sensor and get the data
  // deviceDHT22.computeHumidity(); //Compute RH
  // tempLoRa = deviceDHT22.computeTemperature(); //Compute temperature and assign it to tempLoRa
  // deviceDHT22.print(); //print
  // delay(3000);
  //heartRateLoRa = heartRate(); //Run the heart beat function and assign the value to hearRateLoRa
  //sendLoRa(1,tempLoRa, ppmLoRa, heartRateLoRa); //Use all the values and send the packet through LoRa.
  while(software_connection.available())//While there are characters to come from the GPS
  {
    Serial.print(software_connection.read());//This feeds the serial NMEA data into the library one char at a time
  }
}
