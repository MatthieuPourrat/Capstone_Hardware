
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

void MAX30102::writeRegister(uint8_t reg, uint8_t data)
{
    Wire.beginTransmission(SLAVE_ADDR); 
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}   

uint8_t MAX30102::readRegister(uint8_t reg)
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

void MAX30102::readRegisterFIFO()
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

void MAX30102::enable_spo2()
{
    this->writeRegister(MODE_CONFIGURATION, 0x03);
    this->writeRegister(SPO2_CONFIGURATION,0x27);
    this->writeRegister(0x0C,0x24);
    this->writeRegister(0x0D,0x24);
}

void MAX30102::clearFIFO()
{
    this->writeRegister(0x04,0x00);
    this->writeRegister(0x05,0x00);
    this->writeRegister(0x06,0x00);
}

uint32_t MAX30102::getIR()
{
    ir = answers[0];
    for(int i = 1; i < 3; i++)
    {
        ir <<= 8;
        ir |= answers[i];
    }
    ir &= 0x3FFFF;
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
    red &= 0x3FFFF;
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
        Serial.println(ir);
        Serial.println(red);
        if (ir < 2000 && red < 2000)
            hr = -10000.00;
        else
            hr = ((float)ir/(float)red)*63;
        sum = sum + hr;
        count++;
    }
    return sum/count;  
}

int MAX30102::HR()
{
    float start = micros();
    int counter = 0;
    float sum = 0.00;
    while(micros() - start < 10000000)
    {
        sum += this->computeHR();
        counter++;
    }
    return sum/counter;
}
