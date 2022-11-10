#include "teros_12.h"
#include "elapsedMillis.h"

#define TEROS12_MEASURE_DELAY 500
#define TEROS12_READ_DELAY 500

float parseFloat(const char *data, size_t head, size_t tail, uint16_t &lastIndex)
{
    float v = 0;
    bool neg = false;
    float dec = 0;
    uint16_t i;
    for (i = head; i <= tail; i++)
    {
        if (i == head)
        {
            if (data[i] == '-')
            {
                neg = true;
                continue;
            }
            else if (data[i] == '+')
                continue;
        }

        if (data[i] == '.' && !dec)
        {
            dec = 10;
            continue;
        }

        if (data[i] >= '0' && data[i] <= '9')
        {
            if (!dec)
                v = v * 10 + data[i] - '0';
            else
            {
                v = v + ((data[i] - '0') / dec);
                dec *= 10;
            }
        }
        else
        {
            break;
        }
    }
    lastIndex = i;
    return v * (neg ? -1 : 1);
}

bool parseData(const char *data, float *dst, int count)
{
    uint16_t head = 0;
    uint16_t tail = strlen(data);

    int idx = 0;
    uint16_t lastIdx = 0;
    for (int i = 0; i < count; i++)
    {
        dst[idx++] = parseFloat(data, head, tail, lastIdx);
        head = lastIdx;
        if (idx >= count)
            break;
    }

    return idx == count;
}
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
void Teros12::setup(uint32_t pollingRate)
{
    this->pollingRate = pollingRate;
    clearBuffer();
}
void Teros12::sendMeasure()
{
    // aM0!
    char cmd[] = "aM!";
    cmd[0] = address;
    sdi->sendCommand((const char *)cmd);
}

void Teros12::sendRead()
{
    // aD0!
    char cmd[] = "aD0!";
    cmd[0] = address;
    sdi->sendCommand((const char *)cmd);
}
uint8_t Teros12::readData()
{
    // validate string
    // 0+1829.42+26.2+0

    // Serial.println((const char *)rBuffer);
    float pdata[4];
    if (parseData((const char *)rBuffer, pdata, 4))
    {
        this->calibratedCountsVWC = pdata[1];
        this->temperature = pdata[2];
        this->electricalConductivity = pdata[3];

        // Serial.printf("[0]: %0.3f [1]: %0.3f [2]: %0.3f\r\n", this->calibratedCountsVWC, this->temperature, this->electricalConductivity);
        return 1;
    }

    return 0;
}

int Teros12::readToBuffer()
{
    elapsedMillis eRead = 0;
    int dataRead = 0;
    while (sdi->available() && eRead < 200)
    {
        dataRead++;
        uint8_t c = sdi->read();
        if (rBufferIndex >= sizeof(rBuffer))
            continue;

        rBuffer[rBufferIndex++] = c;
    }
    return dataRead;
}
uint8_t Teros12::tick()
{

    switch (step)
    {
    case 0:
    {
        // Serial.println(__LINE__);
        step++;
        break;
    }
    case 1: // wait next sample cycle
    {
        if (timer >= pollingRate)
        {
            timer = 0;
            clearBuffer();
            sendMeasure();
            e = 0;
            // Serial.println(__LINE__);
            step++;
        }

        break;
    }
    case 2: // wait reply
    {

        readToBuffer();
        if (e >= TEROS12_MEASURE_DELAY)
        {
            // Serial.println(__LINE__);
            step++;
        }

        break;
    }

    case 3: // send read
    {
        if (rBufferIndex == 0) // no reply from prev, abort!
        {
            error = 1;
            step = 0;
            break;
        }
        clearBuffer();
        sendRead();
        e = 0;
        // Serial.println(__LINE__);
        step++;

        break;
    }
    case 4: // wait reply
    {
        readToBuffer();
        if (e >= TEROS12_READ_DELAY)
        {
            // Serial.println(__LINE__);
            step++;
        }
        break;
    }
    case 5: // process read
    {
        if (rBufferIndex == 0) // no reply from prev, abort!
            error = 1;
        else if (readData())
            error = 0;
        else
            error = 1;

        step = 0;
        break;
    }
    default:
        step = 0;
        break;
    }

    if (step == 0)
    {

        if (error && onError != nullptr)
            onError(this);
        if (!error && onDataUpdated != nullptr)
            onDataUpdated(this);
    }
    return step;
}
void Teros12::setCallback(Teros12Callback onDataUpdatedCallback, Teros12Callback onErrorCallback)
{
    this->onDataUpdated = onDataUpdatedCallback;
    this->onError = onErrorCallback;
}

// void Teros12::changeAddress(uint8_t newAddress)
// {
//     uint8_t cmd[5] = {address, 'A', newAddress, '!', 0};
//     address = newAddress;
//     sdi->sendCommand((const char *)cmd);
//     elapsedMillis e = 0;
//     while (e < 1000)
//     {
//         while (sdi->available())
//         {
//             Serial.write(sdi->read());
//         }
//     }
// }
