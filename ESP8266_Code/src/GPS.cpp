//Library Created by: Group 13
//Date: March 22, 2023
//Concordia University
//Capstone Group: 13
//This library belongs to Group 13.

//This library interfaces the GPS module with the Microcontroller. It reads the UART port character by character
//Makes a string out these characters. The string is parsed to identify a special marker.
//When this marker is detected the library detects if the GPS returns valid coordinates.
//If the coordinates are valid, they are processed and send through LoRa otherwise generic invalid values are sent.

#include <Arduino.h>
#include "GPS.h"
#include "HardwareSerial.h"

using namespace std;

GPS::GPS() //Initialize the constructor to invalid coordinates
{
    this->latitude = -9999.00;
    this->longitude = -9999.00;
    this->latChar = 'I';
    this->longChar = 'I';
    this->coordinates = "";
}


void GPS::readCoordinates(String c)
{
    int comma = 0;
    int saved_position = 0;
    if(c.charAt(18) == 'A') //Character number 18 returns A for a Valid coordinates, or V for invalid coordinates.
    {
        for(unsigned int i = 20; i < c.length(); i++)
        {
            if(c.charAt(i) == ',' && comma == 0)
            {
                this->latitude = c.substring(20, i); //Parse to get the latitude
                this->latChar=c.charAt(i+1); //Get the letter (N or S)
                comma++;
                i = i + 3;
                saved_position = i;
            }
            else if(c.charAt(i) == ',' && comma == 1)
            {
                this->longitude = c.substring(saved_position, i); //parse to get the longitude
                this->longChar=c.charAt(i+1); //get the letter (W or E)
                break;
            }
        }
    }
    else //If the character 18 is V then no satellite signal has been acquired. Default values representing invalid are sent.
    {
        Serial.println("Acquiring Satellite");
        this->latitude = "-9999.00"; //Coordinate does not exist.
        this->longitude = "-9999.00";
        this->latChar = 'I'; //I for invalid
        this->longChar = 'I';
    }

}

String GPS::getLatitude()
{
    return this->latitude;
}

String GPS::getLongitude()
{
    return this->longitude;
}

char GPS::getLatitudeChar()
{
    return this->latChar;
}

char GPS::getLongitudeChar()
{
    return this->longChar;
}

void GPS::print()
{
    Serial.println("-------------------------GPS------------------------");
    Serial.print("Latitude: ");
    Serial.print(this->latChar);
    Serial.println(this->latitude);
    Serial.print("Longitude: ");
    Serial.print(this->longChar);
    Serial.println(this->longitude);
    Serial.println("----------------------------------------------------");
}

