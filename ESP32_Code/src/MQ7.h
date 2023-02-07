#include <Arduino.h>

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