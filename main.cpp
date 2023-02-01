#include <Arduino.h>
#include "MQ7.h"
#include <iostream>

#define digitalDHT 32


unsigned long lastTime = 0;
int bitTime, bit = 0;
int dataBuffer[40] = {};
int humidityBuffer[16], temperatureBuffer[16] = {};
int checksumBuffer[8] = {};
double humidity, temperatur = 0.0;

void temperature()
{
  humidity = 0.0;
  temperatur = 0.0;
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

  for(int i = 0; i < 16; i++)
    humidityBuffer[i] = dataBuffer[i];
  
  for(int i = 16; i < 32; i++)
    temperatureBuffer[i-16] = dataBuffer[i];
  
  for(int i = 32; i < 40; i++)
    checksumBuffer[i-32] = dataBuffer[i];

  for(int i = 0; i < 16; i++)
    humidity = humidity + (humidityBuffer[i] * pow(2,16-i-1));
    
  for(int i = 0; i < 16; i++)
    temperatur += temperatureBuffer[i] * pow(2,16-i-1);

  Serial.print("The humidity level is: ");
  Serial.println(humidity/10);

  Serial.print("The temperature level is: ");
  Serial.println(temperatur/10);

  Serial.println("---------------------------------------------");
  delay(3000);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Program has started.");
}

void loop() {
  //MQ7 deviceMQ7;
  //deviceMQ7.computeRs();
  //deviceMQ7.computePPM();
  //deviceMQ7.printMQ7();
  temperature();

}
