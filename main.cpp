#include <Arduino.h>
#include "MQ7.h"
#include <iostream>

#define digitalDHT 32


float lastTime = 0;
int bitTime, bit = 0;
int dataBuffer[40] = {};
int humidityBuffer[16], temperatureBuffer[16] = {};
int checksumBuffer[8] = {};
int humidity, temperatur = 0.0;

void temperature()
{
  Serial.println("---------------Temperature-------------------");
  pinMode(digitalDHT, OUTPUT);
  digitalWrite(digitalDHT, HIGH);
  delay(12);
  digitalWrite(digitalDHT, LOW);
  delay(0.04);
  pinMode(digitalDHT, INPUT);

  lastTime = micros();
  while(micros() - lastTime < 80 && digitalRead(digitalDHT) == 0){Serial.print(digitalRead(digitalDHT));};
  Serial.println();
  lastTime = micros();
  while(micros() - lastTime < 80 && digitalRead(digitalDHT) == 1){Serial.print(digitalRead(digitalDHT));};
  Serial.println();
  for(int i = 0; i < 40; i++)
  {
    lastTime = micros();
    while(micros() - lastTime < 50 && digitalRead(digitalDHT) == 0){Serial.print(digitalRead(digitalDHT));};;

    lastTime = micros();
    while(digitalRead(digitalDHT) == 1){Serial.print(digitalRead(digitalDHT));};
    bitTime = micros() - lastTime;
    if(26 < bit < 28)
      bit = 0;
    else
      bit = 1;
    dataBuffer[i] = bit;
  }

  Serial.print("The temperature is: ");

  for(int i = 0; i < 16; i++)
  {
    humidityBuffer[i] = dataBuffer[i];
    humidity = (unsigned)humidityBuffer[i] << 1;
  }
    
  
  for(int i = 16; i < 32; i++)
  {
    temperatureBuffer[i] = dataBuffer[i];
    temperatur = (unsigned)temperatureBuffer[i] << 1;
  }
  for(int i = 32; i < 40; i++)
    checksumBuffer[i] = dataBuffer[i];

  for(int i = 32; i < 40; i++)
    Serial.print(dataBuffer[i]);

  Serial.println(temperatur);

  Serial.println("---------------------------------------------");
  delay(2000);

}

void setup() {
  Serial.begin(9600);
  Serial.println("Program has started.");
  //pinMode(analogMQ, INPUT);
  //pinMode(digitalPIN, INPUT);
}

void loop() {
  //MQ7 deviceMQ7;
  //deviceMQ7.computeRs();
  //deviceMQ7.computePPM();
  //deviceMQ7.printMQ7();
  temperature();

}