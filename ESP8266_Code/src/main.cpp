#include <Arduino.h>
#include "MQ7.h"
#include "DHT22.h"
#include <iostream>
#include <Wire.h>
#include "LoRa.h" //https://github.com/sandeepmistry/arduino-LoRa
#include <string.h>
#include "SoftwareSerial.h"
#include "GPS.h"
#include "MAX30102.h"

//Pin definition for LoRa
#define ss 15
#define rst 0
#define dio0 16
//

SoftwareSerial software_connection(9,10);

GPS deviceGPS;
MQ7 deviceMQ7; //New MQ& object
DHT22 deviceDHT22; //New DHT22 object
MAX30102 deviceMAX;

char c_coordinates;
String NMEA;


float ppmLoRa,tempLoRa, heartRateLoRa = 0.0; //Variables for the LoRa transmission
String latLoRa, longLoRa;
char latCharLoRa, longCharLoRa;

void getCoordinates()
{
  while(software_connection.available() > 0)
  {
    c_coordinates = software_connection.read();
    if(c_coordinates == '$' && NMEA.substring(0,7) == "$GNRMC,")
    {
      deviceGPS.readCoordinates(NMEA);
      NMEA = "";
      break;
    } 
    else if(c_coordinates == '$')
      NMEA = "";
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
  LoRa.print(id);
  LoRa.print("a");
  LoRa.print(temperaturLoRa);
  LoRa.print("b");
  LoRa.print(ppm);
  LoRa.print("c");
  LoRa.print(heartRate);
  LoRa.print("x");
  LoRa.print(latitude);
  LoRa.print(latChar);
  LoRa.print("y");
  LoRa.print(longitude);
  LoRa.print(longChar);
  LoRa.print("\r\n");
  LoRa.endPacket();

  Serial.println();
  delay(5000);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Program has started.");

  software_connection.begin(9600);
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
  while(micros() - lastTime < 10)
    heartRate(); //Run the heart beat function and assign the value to hearRateLoRa
  getCoordinates();
  sendLoRa(1,tempLoRa, ppmLoRa, heartRateLoRa, deviceGPS.getLatitude(), deviceGPS.getLongitude(),deviceGPS.getLatitudeChar(), deviceGPS.getLongitudeChar()); //Use all the values and send the packet through LoRa.
  //sendLoRa(1,22.00, 1.2, 72.00, 45.1234,87.1923,'N', 'W'); //Use all the values and send the packet through LoRa.
}
