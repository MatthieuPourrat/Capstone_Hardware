#include <Arduino.h>
#include <iostream>

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
        void print();
    private:
        String latitude, longitude;
        char latChar, longChar;
        String coordinates;
        

};