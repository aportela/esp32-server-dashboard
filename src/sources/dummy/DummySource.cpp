#include "DummySource.hpp"
#include <Arduino.h>

DummySource::DummySource(void)
{
    this->currentCPU = new SourceDataCPU();
    this->currentMemory = new SourceDataMemory();
    this->currentTemperature = new SourceDataTemperature();
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
    this->currentCPU->globalCPULoad = random(10, 90);
    this->currentMemory->totalMemory = 32; // 32000000000; // 32 Gbytes (to bytes)
    this->currentMemory->usedMemory = random(this->currentMemory->totalMemory / 3, this->currentMemory->totalMemory);
    this->currentTemperature->globalTemperature = random(20, 80);
}

DummySource::~DummySource()
{
    delete this->currentCPU;
    this->currentCPU = nullptr;
    delete this->currentMemory;
    this->currentMemory = nullptr;
    delete this->currentTemperature;
    this->currentTemperature = nullptr;
}

SourceDataCPU DummySource::getCurrentCPU(void)
{
}

SourceDataMemory DummySource::getCurrentMemory(void)
{
}

SourceDataNetwork DummySource::getCurrentNetwork(void)
{
}

uint8_t DummySource::getCurrentCPULoad(void)
{
    if (random(0, 20) % 2 == 0)
    {
        if (this->currentCPU->globalCPULoad < 99)
        {
            this->currentCPU->globalCPULoad++;
        }
    }
    else if (this->currentCPU->globalCPULoad > 1)
    {
        this->currentCPU->globalCPULoad--;
    }
    return (this->currentCPU->globalCPULoad);
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

SourceData DummySource::getCurrent(SourceDataType entity)
{

    switch (entity)
    {
    case SDT_CPU:
        return (this->getCurrentCPU());
        break;
    case SDT_MEMORY:
        return (this->getCurrentMemory());
        break;
    case SDT_NETWORK:
        return (this->getCurrentNetwork());
        break;
        /*
    case SDT_TEMPERATURE:
        return (this->getCurrentTemperature());
        break;
        */
    }
}
