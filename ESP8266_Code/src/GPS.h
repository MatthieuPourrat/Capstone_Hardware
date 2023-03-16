#include <Arduino.h>
#include <iostream>
#include <string>

class GPS
{
    public:
        GPS();
        void readCoordinates(char c);
        float getLatitude();
        float getLongitude();
        char getLatitudeChar();
        char getLongitudeChar();
    private:
        float latitude, longitude;
        char latChar, longChar;
        std::string coordinates;
        

};