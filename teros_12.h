#ifndef _teros_12_h
#define _teros_12_h
#include "Arduino.h"
#include "SDI12.h"
class Teros12
{
public:
    float calibratedCountsVWC;
    float temperature;
    float electricalConductivity;

    uint8_t address = '0';
    SDI12 *sdi;
    Teros12(SDI12 *sdi);
    uint8_t tick();
    void changeAddress(uint8_t newAddress);
};

#endif