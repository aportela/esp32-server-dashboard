#include "DummySource.hpp"
#include <Arduino.h>

DummySource::DummySource(void)
{
    randomSeed(analogRead(0) ^ (micros() * esp_random()));

    this->currentCPULoad = new SourceDataCPULoad(MIN_CPU_LOAD, MAX_CPU_LOAD);
    this->currentCPULoad->setCurrent(random(MIN_CPU_LOAD, MAX_CPU_LOAD));

    this->currentMemory = new SourceDataMemory();
    this->currentTemperature = new SourceDataTemperature();

    this->currentMemory->totalMemory = 32; // 32000000000; // 32 Gbytes (to bytes)
    this->currentMemory->usedMemory = random(this->currentMemory->totalMemory / 3, this->currentMemory->totalMemory);
    this->currentTemperature->globalTemperature = random(20, 80);
}

DummySource::~DummySource()
{
    delete this->currentCPULoad;
    this->currentCPULoad = nullptr;
    delete this->currentMemory;
    this->currentMemory = nullptr;
    delete this->currentTemperature;
    this->currentTemperature = nullptr;
}

uint8_t DummySource::getCurrentCPULoad(void)
{
    uint8_t current = this->currentCPULoad->getCurrent();
    if (random(0, 20) % 2 == 0)
    {
        if (current < MAX_CPU_LOAD)
        {
            current++;
        }
    }
    else if (current > MIN_CPU_LOAD)
    {
        current--;
    }
    this->currentCPULoad->setCurrent(current);
    return (current);
}

uint8_t DummySource::getCurrentTemperature(void)
{
    if (random(0, 20) % 2 == 0)
    {
        if (this->currentTemperature->globalTemperature < 99)
        {
            this->currentTemperature->globalTemperature++;
        }
    }
    else if (this->currentTemperature->globalTemperature > 1)
    {
        this->currentTemperature->globalTemperature--;
    }
    return (this->currentTemperature->globalTemperature);
}

uint64_t DummySource::getTotalMemory(void)
{
    return (this->currentMemory->totalMemory);
}

uint64_t DummySource::getUsedMemory(void)
{
    if (random(0, 20) % 2 == 0)
    {
        if (this->currentMemory->usedMemory < this->currentMemory->totalMemory)
        {
            this->currentMemory->usedMemory++;
        }
    }
    else if (this->currentMemory->usedMemory > 1)
    {
        this->currentMemory->usedMemory--;
    }
    return (this->currentMemory->usedMemory);
}
