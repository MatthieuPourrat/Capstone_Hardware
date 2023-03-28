//Library Created by: Group 13
//Date: February 7, 2023
//Concordia University
//Capstone Group: 13
//This library belongs to Group 13.
#include <Arduino.h>
#include <iostream>

#define digitalDHT 32

using namespace std;

class DHT22{
    //general buffers to store the data before processing.
    private:
        int dataBuffer[40] = {};
        int humidityBuffer[16], temperatureBuffer[16] = {};
        int checksumBuffer[8] = {};
        double humidity, temperatur = 0.0;
        unsigned long lastTime = 0;
        int bitTime, bit = 0;

    public:
        DHT22();
        ~DHT22();
        void readSensor();
        float computeHumidity();
        float computeTemperature();
        void print();
};