#include <Arduino.h>
#include <iostream>
#include <string>

class GPS
{
    public:
        float latitude, longitude;
        std::string coordinates;
    private:
        void readCoordinates(char c);

};