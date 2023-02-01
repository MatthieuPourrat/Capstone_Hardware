#include <Arduino.h>

#define analogMQ 35
#define digitalMQ 34
#define vin 5 //mV
#define Rl 10000
#define a 100
#define b -1.53

using namespace std;

class MQ7{
    private:
        double Rs, ppm = 0.0;

    public:
        MQ7();
        double computeRs();
        double computePPM();
        void printMQ7();
};