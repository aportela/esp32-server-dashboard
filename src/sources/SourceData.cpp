
#include "SourceData.hpp"
#include <Arduino.h>
#include <cstdio>

#define MIN_CPU_LOAD 0
#define MAX_CPU_LOAD 100

SourceData::SourceData(uint64_t totalMemoryBytes)
{
    this->totalMemoryBytes = totalMemoryBytes;
}

SourceData::~SourceData()
{
}

uint8_t SourceData::getMinCPULoad(void) const
{
    return (MIN_CPU_LOAD);
}

uint8_t SourceData::getMaxCPULoad(void) const
{
    return (MAX_CPU_LOAD);
}

float SourceData::getCurrentCPULoad(void) const
{
    return (this->currentCPULoad);
}

void SourceData::parseCurrentCPULoad(char *buffer, size_t buffer_size, uint8_t decimals) const
{
    char format[decimals + 3] = {'\0'};
    std::snprintf(format, sizeof(format), "%%0%dd", decimals);
    Serial.println(format);
    std::snprintf(buffer, buffer_size, format, this->getCurrentCPULoad());
    Serial.println(buffer);
}

uint64_t SourceData::getCurrentCPULoadTimestamp(void) const
{
    return (this->currentCPULoadTimestamp);
}

bool SourceData::changedCPULoad(uint64_t fromTimestamp) const
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

uint64_t SourceData::getTotalMemoryBytes(void) const
{
    return (this->totalMemoryBytes);
}

uint64_t SourceData::getUsedMemoryBytes(void) const
{
    return (this->usedMemoryBytes);
}

bool SourceData::changedUsedMemoryBytes(uint64_t fromTimestamp) const
{
    return (fromTimestamp != this->currentUsedMemoryTimestamp);
}

bool SourceData::setUsedMemoryBytes(uint64_t bytes, uint64_t timestamp)
{
    if (bytes != this->usedMemoryBytes)
    {
        if (bytes >= 0 && bytes <= this->totalMemoryBytes)
        {
            this->usedMemoryBytes = bytes;
            this->currentUsedMemoryTimestamp = timestamp;
            return (true);
        }
        else
        {
            return (false);
        }
    }
    else
    {
        this->currentUsedMemoryTimestamp = timestamp;
    }
}
