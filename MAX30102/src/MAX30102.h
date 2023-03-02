#include <iostream>

using namespace std;


//Slave Address
#define SLAVE_ADDR 0x57

//Registers
#define FIFO_WR_PTR 0x04
#define OVF_COUNTER 0x05
#define FIFO_RD_PTR 0x06
#define FIFO_DATA 0x07
#define MODE_CONFIGURATION 0x09
#define SPO2_CONFIGURATION 0X0A

//Modes
#define HR_MODE 0x02


class MAX30102
{ 
    public:
        MAX30102();
        void writeRegister(uint8_t reg, uint8_t data);
        uint8_t readRegister(uint8_t reg);
        void enable_spo2();
        void clearFIFO();
        uint8_t readRegisterFIFO()
    private:
};