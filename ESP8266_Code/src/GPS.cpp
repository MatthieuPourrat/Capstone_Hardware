#include <Arduino.h>
#include "GPS.h"

GPS::GPS()
{
    this->latitude = 0.00;
    this->longitude = 0.00;
}

void GPS::readCoordinates(char c)
{
    coordinates.push_back(c);
    if(coordinates.substr(0,6) == "$GNRMC," && coordinates.back() == '/')
    {
        
    }

}