//Library Created by: Group 13
//Date: February 7, 2023
//Concordia University
//Capstone Group: 13
//This library belongs to Group 13.
#include <Arduino.h>
#include <iostream>
#include "MQ7.h"

using namespace std;

MQ7::MQ7() //Constructor
{
    this->Rs = 0.0;
    this->ppm = 0.0;
}

float MQ7::computeRs() //Compute the value of the surface resistance
{
    //Rs is computed using the formula on the sensor data sheet.
    Rs = (vin - (analogRead(analogMQ) * pow(10,-3)))/(analogRead(analogMQ) * pow(10,-3));
    Rs = Rs * Rl;
    return Rs;
}

float MQ7::computePPM() //Compute the PPM levels of CO
{
    ppm = a * pow((Rs/Ro), b); //Refer to MQ7.h for more explanations.
    return ppm;
}

void MQ7::printMQ7()
{
    Serial.println("---------------Carbon Monoxide--------------");
    Serial.print("The ppm reading is: ");
    Serial.println(ppm);
/*
    Serial.print("The digital value is: ");
    Serial.println(digitalRead(digitalMQ));

    if(digitalRead(digitalMQ) == HIGH)
        Serial.println("CO detected.");
    else
        Serial.println("CO not detected.");
*/
    Serial.println("---------------------------------------------");

    delay(1000);
}
