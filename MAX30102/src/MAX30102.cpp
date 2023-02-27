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