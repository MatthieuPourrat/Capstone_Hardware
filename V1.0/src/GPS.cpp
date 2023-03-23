#include <Arduino.h>
#include "GPS.h"
#include "HardwareSerial.h"

using namespace std;

GPS::GPS()
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
    if(c.charAt(18) == 'A')
    {
        for(unsigned int i = 20; i < c.length(); i++)
        {
            if(c.charAt(i) == ',' && comma == 0)
            {
                this->latitude = c.substring(20, i);
                this->latChar=c.charAt(i+1);
                comma++;
                i = i + 3;
                saved_position = i;
            }
            else if(c.charAt(i) == ',' && comma == 1)
            {
                this->longitude = c.substring(saved_position, i);
                this->longChar=c.charAt(i+1);
                break;
            }
        }
    }
    else
    {
        Serial.println("Acquiring Satellite");
        this->latitude = "-9999.00";
        this->longitude = "-9999.00";
        this->latChar = 'I';
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

