#include <Arduino.h>
#include <iostream>
#include "SoftwareSerial.h"

using namespace std;

class GPS
{
    public:
        GPS();
        void readCoordinates(String c);
        String getLatitude();
        String getLongitude();
        char getLatitudeChar();
        char getLongitudeChar();
    private:
        String latitude, longitude;
        char latChar, longChar;
        String coordinates;
        

};