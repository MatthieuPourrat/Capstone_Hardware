//Library Created by: Group 13
//Date: March 22, 2023
//Concordia University
//Capstone Group: 13
//This library is partially the work of team 13.
//It has been developped using previous work done on this sensor.
//Special thanks to our supervisor Dr Rodolfo Countinho for providing us with a sample code in python for this sensor.
//The following library has therefore been developped with the python file.
//That being said, some algorithms are Group 13 creation.

//This library interfaces the MAX30102 sensor to the Microcontroller and samples the IR and RED values 
//of the sensor during 10 seconds. The MAX has the capability to store 32 samples at one time. 
//Therefore, this library reads the FIFO data register 32 samples during 10 seconds.

#include <iostream>
#include <Arduino.h>
#include "MAX30102.h"
#include <Wire.h>

MAX30102::MAX30102()
{
    ir = 0;
    red = 0;
    hr = 0.00;
}

void MAX30102::writeRegister(uint8_t reg, uint8_t data) //Team 13 and Dr. Coutinho's file 50/50
{
    Wire.beginTransmission(SLAVE_ADDR); 
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}   

uint8_t MAX30102::readRegister(uint8_t reg) //Team 13 and Dr. Coutinho's file 50/50
{
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(SLAVE_ADDR, 1); 
    if(Wire.available())
        return Wire.read();
    else
        return -1;
}

void MAX30102::readRegisterFIFO() //Team 13 and Dr. Coutinho's file 50/50
{
    int counter = 0;
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(FIFO_DATA);
    Wire.endTransmission(false);
    Wire.requestFrom(SLAVE_ADDR, 6); 
    while(Wire.available())
    {   
        answers[counter] = Wire.read();
        counter++;
    }
}

void MAX30102::enable_spo2() //The configuration values are taken from Dr. Coutinho's python file for the MAX30102
{
    this->writeRegister(MODE_CONFIGURATION, 0x03);
    this->writeRegister(SPO2_CONFIGURATION,0x27);
    this->writeRegister(0x0C,0x24);
    this->writeRegister(0x0D,0x24);
}

void MAX30102::clearFIFO() //Team 13 & Datasheet
{
    this->writeRegister(0x04,0x00);
    this->writeRegister(0x05,0x00);
    this->writeRegister(0x06,0x00);
}

uint32_t MAX30102::getIR() //Team 13 and Dr. Coutinho's file 50/50
{
    ir = answers[0];
    for(int i = 1; i < 3; i++)
    {
        ir <<= 8;
        ir |= answers[i];
    }
    ir &= 0x3FFFF; //Will mask bit 23 to 18 since they are reserved
    return ir;
}

uint32_t MAX30102::getRed()
{
    red = answers[3];
    for(int i = 4; i < 6; i++)
    {
        red <<= 8;
        red |= answers[i];
    }
    red &= 0x3FFFF; //Will mask bit 23 to 18 since they are reserved
    return red;
}

float MAX30102::computeHR()
{
    float sum = 0.0;
    int count = 0;
    while(count != 32)
    {
        this->readRegisterFIFO();
        this->getRed();
        this->getIR();
        if (ir < 2000 || red < 2000) //if the readings are below 2000 then the heart rate is invalid.
            return hr = -10000.00;
        else
            hr = ((float)ir/(float)red)*63; //Formula from Dr. Coutinho's file 50/50
        sum = sum + hr;
        count++;
    }
    return sum/count;  
}

float MAX30102::HR() //Team 13
{
    float start = micros();
    int counter = 0;
    float sum = 0.00;
    while(micros() - start < 10000000) //Run the loop for 10 seconds.
    {
        sum += this->computeHR();
        counter++;
    }
    return sum/counter;
}
