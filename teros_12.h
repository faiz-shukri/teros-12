#ifndef _teros_12_h
#define _teros_12_h
#include "Arduino.h"
#include "SDI12.h"
class Teros12
{
private:
    uint8_t rBuffer[24];
    uint8_t rBufferIndex = 0;
    uint8_t step;
    elapsedMillis e;
    elapsedMillis timer;
    void clearBuffer();
    void sendMeasure();
    uint8_t measure();
    void sendRead();
    uint8_t read();
public:
    float calibratedCountsVWC;
    float temperature;
    float electricalConductivity;

    SDI12 *sdi;
    uint8_t address = '0';
    uint32_t pollingRate;

    Teros12(SDI12 *sdi,uint8_t address = '0');
    uint8_t tick();
    void setup(uint32_t pollingRate = 1000);
    void changeAddress(uint8_t newAddress);
};

#endif