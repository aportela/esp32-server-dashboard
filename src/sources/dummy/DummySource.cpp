#include "DummySource.hpp"
#include <Arduino.h>
#include "../../utils/Format.hpp"

DummySource::DummySource(SourceData *sourceData) : Source(sourceData)
{
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
    this->lastEllapsedMillis = millis();
}

DummySource::~DummySource()
{
}

void DummySource::refresh(uint16_t milliSeconds)
{
    uint64_t currentMillis = millis();
    bool allowRefresh = milliSeconds == 0 || ((currentMillis - this->lastEllapsedMillis) >= milliSeconds);
    if (allowRefresh)
    {
        float fCurrent = 0.0f;
        uint64_t current = 0;
        uint64_t change = 0;

        uint8_t rnd = random(0, 100);
        if (rnd > 90)
        {
            fCurrent = random(MIN_CPU_LOAD, MAX_CPU_LOAD * 100);
        }
        else if (rnd > 50)
        {
            fCurrent = random(MIN_CPU_LOAD, (MAX_CPU_LOAD / 5) * 100);
        }
        else
        {
            fCurrent = random(MIN_CPU_LOAD, (MAX_CPU_LOAD / 10) * 100);
        }
        fCurrent /= 100.0f;
        this->sourceData->setCurrentCPULoad(fCurrent, currentMillis);

        current = this->sourceData->getUsedMemory();
        change = this->sourceData->getTotalMemory() / 100;
        if (random(0, 20) % 2 == 0)
        {
            if (current < this->sourceData->getTotalMemory() - change)
            {
                current += change;
            }
        }
        else if (current > change)
        {
            current -= change;
        }
        this->sourceData->setUsedMemory(current, currentMillis);

        current = this->sourceData->getCurrentCPUTemperature();
        if (random(0, 20) % 2 == 0)
        {
            if (current < this->sourceData->getMaxCPUTemperature())
            {
                current++;
            }
        }
        else if (current > (this->sourceData->getMaxCPUTemperature() / 5))
        {
            current--;
        }
        this->sourceData->setCurrentCPUTemperature(current, currentMillis);

        // TODO: check limits
        current = this->sourceData->getCurrentTotalNetworkDownloaded();
        this->sourceData->setCurrentTotalNetworkDownloaded(current + random(1000000, 5000000000), currentMillis);

        current = this->sourceData->getCurrentTotalNetworkUploaded();
        this->sourceData->setCurrentTotalNetworkUploaded(current + random(1000000, 5000000000), currentMillis);

        this->lastEllapsedMillis = currentMillis;
    }
}
