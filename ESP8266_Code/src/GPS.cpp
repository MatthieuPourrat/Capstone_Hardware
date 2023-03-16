#include <Arduino.h>
#include "GPS.h"

GPS::GPS()
{
    this->latitude = 0.00;
    this->longitude = 0.00;
    this->latChar = 'I';
    this->longChar = 'I';
}

void GPS::readCoordinates(char c)
{
    coordinates.push_back(c);
    if(coordinates.substr(0,6) == "$GNRMC," && coordinates.back() == '/')
    {
        int comma = 0;
        int saved_position = 0;
        if(coordinates.at(18) == 'A')
        {
            for(int i = 20; i < coordinates.length(); i++)
            {
                if(coordinates.at(i) == ',' && comma == 0)
                {
                    this->latitude = stof(coordinates.substr(20, i - 1))/100.00;
                    this->latChar=coordinates.at(i+1);
                    comma++;
                    i = i + 3;
                    saved_position = i;
                }
                else if(coordinates.at(i) == ',' && comma == 1)
                {
                    this->longitude = stof(coordinates.substr(saved_position, i - 1))/100.00;
                    this->longChar=coordinates.at(i+1);
                    break;
                }
                
            }
        }
        else
        {
            this->latitude = -9999.00;
            this->longitude = -9999.00;
            this->latChar = 'I';
            this->longChar = 'I';
        }
    }
}

float GPS::getLatitude()
{
    return this->latitude;
}

float GPS::getLongitude()
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

