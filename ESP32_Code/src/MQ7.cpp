#include <Arduino.h>
#include <iostream>
#include "MQ7.h"

using namespace std;

MQ7::MQ7()
{
    this->Rs = 0.0;
    this->ppm = 0.0;
}

float MQ7::computeRs()
{
    Rs = (vin - (analogRead(analogMQ) * pow(10,-3)))/(analogRead(analogMQ) * pow(10,-3));
    Rs = Rs * Rl;
    return Rs;
}

float MQ7::computePPM()
{
    ppm = a * pow((Rs/Ro), b);
    return ppm;
}

void MQ7::printMQ7()
{
    Serial.println("---------------Carbon Monoxide--------------");
    Serial.print("The ppm reading is: ");
    Serial.println(ppm);

    Serial.print("The digital value is: ");
    Serial.println(digitalRead(digitalMQ));

    if(digitalRead(digitalMQ) == HIGH)
        Serial.println("CO detected.");
    else
        Serial.println("CO not detected.");
    
    Serial.println("---------------------------------------------");

    delay(1000);
}
