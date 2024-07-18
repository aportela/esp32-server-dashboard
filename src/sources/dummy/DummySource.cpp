#include "DummySource.hpp"
#include <Arduino.h>

DummySource::DummySource(void)
{
    this->currentTemperature = new SourceDataTemperature();
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
    this->currentTemperature->globalTemperature = random(20, 99);
}

DummySource::~DummySource()
{
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
