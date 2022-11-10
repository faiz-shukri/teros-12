#include "teros_12.h"
#include "elapsedMillis.h"

#define TEROS12_MEASURE_DELAY 100
Teros12::Teros12(SDI12 *sdi, uint8_t address)
{
    this->sdi = sdi;
    this->address = address;
}
void Teros12::clearBuffer()
{
    memset(rBuffer, 0, sizeof(rBuffer));
    rBufferIndex = 0;
}
void Teros12::setup(uint32_t pollingRate = 1000)
{
    this->pollingRate = pollingRate;
    clearBuffer();
}
void Teros12::sendMeasure()
{
    // aM0!
    sdi->write(address);
    sdi->print("M!");
}
uint8_t Teros12::measure()
{
}
void Teros12::sendRead()
{
    // aD0!
    sdi->write(address);
    sdi->print("D0!");
}
uint8_t Teros12::read()
{
}
uint8_t Teros12::tick()
{
    /*
    switch (step)
    {
    case 0:
    {
        step++;
        break;
    }
    case 1: // wait next sample cycle
    {
        if (timer >= pollingRate)
        {
            timer = 0;
            sendMeasure();
            e = 0;
            step++;
        }

        break;
    }
    case 2: // wait measurement completed
    {
        if (e >= HTU21D_READ_DELAY)
        {
            uint8_t res = readTemperature();
            temperatureError = res;
            if (res)
                step = 0;
            else
                step++;
        }
        break;
    }
    case 3: // wait measurement completed
    {
        uint8_t res = requestHumidity();
        e = 0;
        if (res)
        {
            humidityError = res;
            step = 0;
        }
        else
            step++;

        break;
    }
    case 4: // wait measurement completed
    {
        if (e >= HTU21D_READ_DELAY)
        {
            uint8_t res = readHumidity();
            humidityError = res;
            step = 0; // step++;
        }
        break;
    }
    default:
        step = 0;
        break;
    }

    if (step == 0)
    {
        error = temperatureError | humidityError;
        if (error && onReadError != nullptr)
            onReadError(this);
        if (!error && onDataUpdated != nullptr)
            onDataUpdated(this);
    }
    return step;
    */
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