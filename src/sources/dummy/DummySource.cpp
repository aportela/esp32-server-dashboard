#include "DummySource.hpp"
#include <Arduino.h>

DummySource::DummySource(SourceData *sourceData) : Source(sourceData)
{
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
}

DummySource::~DummySource()
{
}

void DummySource::refresh(void)
{
    uint64_t current = 0;
    uint64_t change = 0;
    uint64_t currentMillis = millis();

    uint8_t rnd = random(0, 100);
    if (rnd > 90)
    {
        current = random(MIN_CPU_LOAD, MAX_CPU_LOAD);
    }
    else if (rnd > 50)
    {
        current = random(MIN_CPU_LOAD, MAX_CPU_LOAD / 5);
    }
    else
    {
        current = random(MIN_CPU_LOAD, MAX_CPU_LOAD / 10);
    }
    this->sourceData->setCurrentCPULoad(current, millis());

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
    this->sourceData->setUsedMemory(current, millis());

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
    this->sourceData->setCurrentCPUTemperature(current, millis());

    current = this->sourceData->getCurrentTotalNetworkDownloaded();
    this->sourceData->setCurrentTotalNetworkDownloaded(current + random(1000000, 5000000000), millis());

    current = this->sourceData->getCurrentTotalNetworkUploaded();
    this->sourceData->setCurrentTotalNetworkUploaded(current + random(1000000, 5000000000), millis());
}
