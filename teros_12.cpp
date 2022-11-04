#include "teros_12.h"
#include "elapsedMillis.h"
Teros12::Teros12(SDI12 *sdi)
{
    this->sdi = sdi;
}

uint8_t Teros12::tick()
{
}

void Teros12::changeAddress(uint8_t newAddress)
{
    uint8_t cmd[5] = {address, 'A', newAddress, '!', 0};
    address = newAddress;
    sdi->sendCommand((const char *)cmd);
    elapsedMillis e = 0;
    while (e < 1000)
    {
        while (sdi->available())
        {
            Serial.write(sdi->read());
        }
    }
}