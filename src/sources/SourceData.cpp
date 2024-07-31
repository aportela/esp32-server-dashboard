
#include "SourceData.hpp"
#include <Arduino.h>
#include <cstdio>

SourceData::SourceData(uint8_t min, uint8_t max) : minCPULoad(min), maxCPULoad(max)
{
    // init random seed
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
}

SourceData::~SourceData()
{
}

uint8_t SourceData::getMinCPULoad(void)
{
    return (this->minCPULoad);
}

uint8_t SourceData::getMaxCPULoad(void)
{
    return (this->maxCPULoad);
}

void SourceData::setMinCPULoad(uint8_t value)
{
    this->minCPULoad = value;
}

void SourceData::setMaxCPULoad(uint8_t value)
{
    this->maxCPULoad = value;
}

float SourceData::getCurrentCPULoad(void)
{
    return (this->currentCPULoad);
}

void SourceData::getCurrentCPULoad(char *buffer, size_t buffer_size, uint8_t decimals)
{
    char format[decimals + 3] = {'\0'};
    std::snprintf(format, sizeof(format), "%%0%dd", decimals);
    Serial.println(format);
    std::snprintf(buffer, buffer_size, format, this->getCurrentCPULoad());
    Serial.println(buffer);
}

float SourceData::getPreviousCPULoad(void)
{
    return (this->previousCPULoad);
}

uint64_t SourceData::getCurrentCPULoadTimestamp(void)
{
    return (this->currentCPULoadTimestamp);
}

bool SourceData::changedCPULoad(uint64_t fromTimestamp)
{
    return (fromTimestamp != this->currentCPULoadTimestamp);
}

bool SourceData::setCurrentCPULoad(float value, uint64_t timestamp)
{
    if (value != this->currentCPULoad)
    {
        if (value >= this->minCPULoad && value <= this->maxCPULoad)
        {
            this->previousCPULoad = this->currentCPULoad;
            this->currentCPULoad = value;
            this->currentCPULoadTimestamp = timestamp;
            return (true);
        }
        else
        {
            return (false);
        }
    }
    else
    {
        this->previousCPULoad = this->currentCPULoad;
        this->currentCPULoadTimestamp = timestamp;
    }
}

void SourceData::refresh(void)
{
    this->setCurrentCPULoad((float)random(this->getMinCPULoad(), this->getMaxCPULoad()), millis());
}