
#include "SourceData.hpp"
#include <Arduino.h>
#include <cstdio>

#define MIN_CPU_LOAD 0
#define MAX_CPU_LOAD 100

SourceData::SourceData(uint8_t min, uint8_t max)
{
    // init random seed
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
}

SourceData::~SourceData()
{
}

uint8_t SourceData::getMinCPULoad(void)
{
    return (MIN_CPU_LOAD);
}

uint8_t SourceData::getMaxCPULoad(void)
{
    return (MAX_CPU_LOAD);
}

float SourceData::getCurrentCPULoad(void)
{
    return (this->currentCPULoad);
}

void SourceData::parseCurrentCPULoad(char *buffer, size_t buffer_size, uint8_t decimals)
{
    char format[decimals + 3] = {'\0'};
    std::snprintf(format, sizeof(format), "%%0%dd", decimals);
    Serial.println(format);
    std::snprintf(buffer, buffer_size, format, this->getCurrentCPULoad());
    Serial.println(buffer);
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
        if (value >= MIN_CPU_LOAD && value <= MAX_CPU_LOAD)
        {
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
        this->currentCPULoadTimestamp = timestamp;
    }
}
