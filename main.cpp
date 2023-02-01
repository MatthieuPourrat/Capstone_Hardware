#include <Arduino.h>
#include "MQ7.h"
#include <iostream>

#define digitalDHT 32


unsigned long lastTime = 0;
int bitTime, bit = 0;
int dataBuffer[40] = {};
int humidityBuffer[16], temperatureBuffer[16] = {};
int checksumBuffer[8] = {};
int humidity, temperatur = 0.0;

void temperature()
{
  int counter = 0;
  Serial.println("---------------Temperature-------------------");
  pinMode(digitalDHT, OUTPUT);
  digitalWrite(digitalDHT, HIGH);
  delay(250);
  digitalWrite(digitalDHT, LOW);
  delay(5);
  digitalWrite(digitalDHT, HIGH);
  delay(0.04);
  pinMode(digitalDHT, INPUT);


  lastTime = micros();
  while((micros()-lastTime < 80) && digitalRead(digitalDHT) == LOW);
  lastTime = micros();
  while((micros()-lastTime < 80) && digitalRead(digitalDHT) == HIGH);

  while(true)
  {
    lastTime = micros();
    while((micros()-lastTime < 50) && digitalRead(digitalDHT) == LOW);
    lastTime = micros();
    while(digitalRead(digitalDHT) == HIGH);
    bitTime = micros() - lastTime;
    if(bitTime < 30)
      bit = 0;
    else
      bit = 1;
    dataBuffer[counter] = bit;
    counter++;
    if(counter == 40)
      break;
  }

  for(int i = 0; i < 40; i++)
    Serial.print(dataBuffer[i]);
  Serial.println();

  for(int i = 40; i > 24; i--)
    humidityBuffer[i-25] = dataBuffer[i];
  
  for(int i = 24; i > 8; i--)
    temperatureBuffer[i-9] = dataBuffer[i];
  
  for(int i = 8; i >0; i--)
    checksumBuffer[i-1] = dataBuffer[i];

  Serial.println("---------------------------------------------");
  delay(3000);

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
