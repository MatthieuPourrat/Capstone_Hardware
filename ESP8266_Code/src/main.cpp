#include <Arduino.h>
#include "MQ7.h"
#include "DHT22.h"
#include <iostream>
#include <Wire.h>
#include "LoRa.h" //https://github.com/sandeepmistry/arduino-LoRa
#include <string.h>
#include "HardwareSerial.h"
#include "GPS.h"
#include "MAX30102.h"

//Pin definition for LoRa
#define ss 5
#define rst 25
#define dio0 33
//


GPS deviceGPS;
MQ7 deviceMQ7; //New MQ& object
DHT22 deviceDHT22; //New DHT22 object
MAX30102 deviceMAX;

//Variables for the GPS.
char c_coordinates;
String NMEA;


float ppmLoRa,tempLoRa, heartRateLoRa = 0.0; //Variables for the LoRa transmission
String latLoRa, longLoRa;
char latCharLoRa, longCharLoRa;

HardwareSerial hardware_serial(2);


void getCoordinates()
{
  while(hardware_serial.available() > 0) //While there is data on the bus.
  {
    c_coordinates = hardware_serial.read();
    if(c_coordinates == '$' && NMEA.substring(0,7) == "$GNRMC,") //if the character is $, it means that we are on a new command.
    //&& if the string from 0 to 6 is equal to $GNRMC then this is the string we need to parse.
    {
      deviceGPS.readCoordinates(NMEA); //Send the string to the library for parsing
      NMEA = ""; //reset the string
      break;
    } 
    else if(c_coordinates == '$') // If just a new NMEA string
      NMEA = ""; //Reset string
    NMEA.concat(c_coordinates);
  }
  latLoRa = deviceGPS.getLatitude();
  longLoRa = deviceGPS.getLongitude();
  latCharLoRa = deviceGPS.getLatitudeChar();
  longCharLoRa = deviceGPS.getLongitudeChar();
  // Serial.println(latLoRa);
  // Serial.println(latCharLoRa);
  // Serial.println(longLoRa);
  // Serial.println(longCharLoRa);
}

void heartRate()
{
  heartRateLoRa = deviceMAX.HR();
}

void sendLoRa(int id, float temperaturLoRa, float ppm, float heartRate, String latitude, String longitude, char latChar, char longChar) //LoRa function to send the packets
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
  Serial.print(", ");
  Serial.print("Latitude: ");
  Serial.print(latChar);
  Serial.print(latitude);
  Serial.print(", ");
  Serial.print("Longitude: ");
  Serial.print(longChar);
  Serial.print(longitude);
  Serial.println(". End of Packet.");
  Serial.println("------------------------------------------");

  // send packet
  LoRa.beginPacket();
  LoRa.print("a");
  LoRa.print(id);
  LoRa.print("b");
  LoRa.print(temperaturLoRa);
  LoRa.print("c");
  LoRa.print(ppm);
  LoRa.print("d");
  LoRa.print(heartRate);
  LoRa.print("x");
  LoRa.print(latitude);
  LoRa.print(latChar);
  LoRa.print("y");
  LoRa.print(longitude);
  LoRa.print(longChar);
  LoRa.endPacket();

  Serial.println();
  delay(5000);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Program has started.");

  //Block 2: LoRa startup from https://github.com/sandeepmistry/arduino-LoRa
  Serial.println("LoRa Starting");
  LoRa.setPins(ss,rst,dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Wire.begin();
  deviceMAX.clearFIFO();
  deviceMAX.enable_spo2();
  //End of Block 2.

  hardware_serial.begin(9600, SERIAL_8N1, GPIO_NUM_16, GPIO_NUM_17);
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
  float lastTime = micros();
  while(micros() - lastTime < 10) // Run the heart beat for 10 seconds.
    heartRate(); //Run the heart beat function and assign the value to hearRateLoRa
  getCoordinates(); //Get the coordinates.



  sendLoRa(1,tempLoRa, ppmLoRa, heartRateLoRa, deviceGPS.getLatitude(), deviceGPS.getLongitude(),deviceGPS.getLatitudeChar(), deviceGPS.getLongitudeChar()); //Use all the values and send the packet through LoRa.
  //sendLoRa(1,tempLoRa, ppmLoRa, heartRateLoRa, "45.49750","73.57870",'N', 'W'); //Use all the values and send the packet through LoRa.
}
