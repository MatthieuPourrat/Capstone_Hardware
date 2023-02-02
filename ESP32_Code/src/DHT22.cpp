#include <Arduino.h>
#include <iostream>
#include "DHT22.h"

using namespace std;

DHT22::DHT22()
{
    this->humidity = 0.0;
    this->temperatur = 0.0;
    this->lastTime=0;
    this->bitTime=0;
    this->bit=0;
}

DHT22::~DHT22(){};

void DHT22::readSensor()
{
    humidity = 0.0;
    temperatur = 0.0;
    int counter = 0;
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
}

void DHT22::computeHumidity()
{
    for(int i = 0; i < 16; i++)
        humidity = humidity + (dataBuffer[i] * pow(2,16-i-1));
}

void DHT22::computeTemperature()
{
    if(dataBuffer[0] == 1)
    {
        for(int i = 17; i < 32; i++)
            temperatur = temperatur + (dataBuffer[i] * pow(2,32-i-1));
        temperatur *= (-1);

    }
    else{
        for(int i = 16; i < 32; i++)
            temperatur = temperatur + (dataBuffer[i] * pow(2,32-i-1));
    }
}

void DHT22::print()
{
    Serial.println();
    Serial.println("--------------------Temperature-----------------");
    Serial.print("Humidity is: ");
    Serial.print(humidity/10);
    Serial.println("%");
    Serial.print("Temperature is: ");
    Serial.print(temperatur/10);
    Serial.println(" degrees Celcius.");
    Serial.println("------------------------------------------------");
    Serial.println();
}

double DHT22::getHumidity()
{
    return humidity/10;
}

double DHT22::getTemperature()
{
    return temperatur/10;
}
