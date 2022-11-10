#ifndef _teros_12_h
#define _teros_12_h
#include "Arduino.h"
#include "SDI12.h"
#include "elapsedMillis.h"
class Teros12;
typedef void (*Teros12Callback)(Teros12 *);
class Teros12
{
private:
    uint8_t rBuffer[24];
    uint8_t rBufferIndex = 0;
    uint8_t step;
    elapsedMillis e;
    elapsedMillis timer;
    void clearBuffer();
    int readToBuffer();
    void sendMeasure();
    void sendRead();
    uint8_t readData();

public:
    float calibratedCountsVWC;
    float temperature;
    float electricalConductivity;

    Teros12Callback onDataUpdated = nullptr;
    Teros12Callback onError = nullptr;

    SDI12 *sdi;
    uint8_t address = '0';
    uint32_t pollingRate;
    uint8_t error = 0;
    Teros12(SDI12 *sdi, uint8_t address = '0');
    uint8_t tick();
    void setup(uint32_t pollingRate = 1000);
    void setCallback(Teros12Callback onDataUpdatedCallback, Teros12Callback onErrorCallback);
    // void changeAddress(uint8_t newAddress);
};

#endif