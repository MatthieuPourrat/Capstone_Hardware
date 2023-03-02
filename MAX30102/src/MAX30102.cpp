#include <iostream>
#include <Arduino.h>
#include "MAX30102.h"
#include <Wire.h>

MAX30102::MAX30102()
{

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

uint8_t MAX30102::readRegisterFIFO()
{
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(FIFO_DATA);
    Wire.endTransmission(false);
    Wire.requestFrom(SLAVE_ADDR, 6); 
    if(Wire.available())
        return Wire.read();
    else
        return -1;
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
