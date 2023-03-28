//Library Created by: Group 13
//Date: February 7, 2023
//Concordia University
//Capstone Group: 13
//This library belongs to Group 13.
#include <Arduino.h>

/*
vin is always 5 Volts
Rl is the load resistance: 10000 Ohms
Ro is the sensor resitance at 2000 Ohms. Value found here: https://forum.arduino.cc/t/new-mq7-library/283971
a is a coefficient: 100
b is a coefficient: -1.53
The a and b coefficient as well as the formula a*(Rs/Ro)^b was found using the formula for logarithmic regression on this website:
https://www.teachmemicro.com/use-mq-7-carbon-monoxide-sensor/ and the data sheet of the MQ7 sensor
Using the logarithmic regression formula, a and b are determined and the formula can be rearanged to calculate the CO PPM.
Calculations were done by Group 13. The coefficients are our values.
*/

#define analogMQ 35
#define digitalMQ 34
#define vin 5 //V
#define Rl 10000 //Ohms
#define Ro 2000 //Ohms
#define a 100
#define b -1.53

using namespace std;

class MQ7{
    private:
        float Rs, ppm = 0.0;

    public:
        MQ7();
        float computeRs();
        float computePPM();
        void printMQ7();
    
};