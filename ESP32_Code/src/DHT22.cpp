//Library Created by: Group 13
//Date: February 7, 2023
//Concordia University
//Capstone Group: 13
//This library belongs to Group 13.
#include <Arduino.h>
#include <iostream>
#include "DHT22.h"

using namespace std;

DHT22::DHT22() //constructor
{
    this->humidity = 0.0;
    this->temperatur = 0.0;
    this->lastTime=0;
    this->bitTime=0;
    this->bit=0;
}
DHT22::~DHT22(){ //destructor
    this->humidity = 0.0;
    this->temperatur = 0.0;
    this->lastTime=0;
    this->bitTime=0;
    this->bit=0;
};

void DHT22::readSensor() //read the data from the sensor.
{
    humidity = 0.0;
    temperatur = 0.0;
    int counter = 0;

    //Start of block: code greatly inspired from https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTstable
    //Adapted to this application.
    pinMode(digitalDHT, OUTPUT);
    digitalWrite(digitalDHT, HIGH); //Write HIGH for 250 milliseconds to prepare the sensor
    delay(250);
    digitalWrite(digitalDHT, LOW); //Write LOW for 5 milliseconds: begging of the start signal
    delay(5);
    digitalWrite(digitalDHT, HIGH); //Pull up to HIGH for 4 microseconds so the sensor knows that it needs to send data.
    delay(0.04);
    pinMode(digitalDHT, INPUT); //Put back the pin to an input.

    //end block

    lastTime = micros();
    //Wait for 80 microseconds and as long as the state is LOW
    //This is the sensor replying to the MCU that it is ready to send the data
    while((micros()-lastTime < 80) && digitalRead(digitalDHT) == LOW);
    lastTime = micros();
    //Wait for 80 microseconds and as long as the state is HIGH
    //This is the sensor replying to the MCU that it is ready to send the data, the signal is 80 micro LOW and 80 micro HIGH
    while((micros()-lastTime < 80) && digitalRead(digitalDHT) == HIGH);
    while(true) //while there is data.
    {
        lastTime = micros();
        while((micros()-lastTime < 50) && digitalRead(digitalDHT) == LOW); //prepare for the next bit
        lastTime = micros();
        while(digitalRead(digitalDHT) == HIGH); //Record the time the state is on HIGH
        bitTime = micros() - lastTime;
        //If the time the state is HIGH is lower than 30microseconds, it is a 0, otherwise a 1.
        if(bitTime < 30)
            bit = 0;
        else
            bit = 1;
        dataBuffer[counter] = bit; //put the bit in the data buffer
        counter++;
        if(counter == 40) //the data is 40 bits longs.
            break;
    }
}

float DHT22::computeHumidity() //Take the first 16 bits and convert them to decimal to get the RH
{
    for(int i = 0; i < 16; i++)
        humidity = humidity + (dataBuffer[i] * pow(2,16-i-1));

    return humidity/10;
}

float DHT22::computeTemperature() //Take the 17 to 32 bits and convert them to decimal to get the temperature
{
    if(dataBuffer[0] == 1) //if the first bit of the sequence is one, the temperature is negative
    {
        for(int i = 17; i < 32; i++)
            temperatur = temperatur + (dataBuffer[i] * pow(2,32-i-1));
        temperatur *= (-1);

    }
    else{
        for(int i = 16; i < 32; i++)
            temperatur = temperatur + (dataBuffer[i] * pow(2,32-i-1));
    }
    return temperatur/10;
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