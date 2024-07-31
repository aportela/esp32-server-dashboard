
#include "SourceData.hpp"
#include <Arduino.h>
#include <cstdio>

#define MIN_CPU_LOAD 0
#define MAX_CPU_LOAD 100

SourceData::SourceData(uint64_t totalMemory, float minCPUTemperature, float maxCPUTemperature, uint64_t totalNetworkDownloadBandwidthLimit)
{
    this->totalMemory = totalMemory;
    this->minCPUTemperature = minCPUTemperature;
    this->maxCPUTemperature = maxCPUTemperature;
    this->totalNetworkDownloadBandwidthLimit = totalNetworkDownloadBandwidthLimit;
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
        return (true);
    }
}

uint64_t SourceData::getTotalMemory(void) const
{
    return (this->totalMemory);
}

uint64_t SourceData::getUsedMemory(void) const
{
    return (this->usedMemory);
}

uint64_t SourceData::getCurrentUsedMemoryTimestamp(void) const
{
    return (this->currentUsedMemoryTimestamp);
}

bool SourceData::changedUsedMemory(uint64_t fromTimestamp) const
{
    return (fromTimestamp != this->currentUsedMemoryTimestamp);
}

bool SourceData::setUsedMemory(uint64_t bytes, uint64_t timestamp)
{
    if (bytes != this->usedMemory)
    {
        if (bytes >= 0 && bytes <= this->totalMemory)
        {
            this->usedMemory = bytes;
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
        return (true);
    }
}

float SourceData::getMinCPUTemperature(void) const
{
    return (this->minCPUTemperature);
}

bool SourceData::setMinCPUTemperature(float celsious)
{
    this->minCPUTemperature = celsious;
    return (true);
}

float SourceData::getMaxCPUTemperature(void) const
{
    return (this->maxCPUTemperature);
}

bool SourceData::setMaxCPUTemperature(float celsious)
{
    this->maxCPUTemperature = celsious;
    return (true);
}

float SourceData::getCurrentCPUTemperature(void) const
{
    return (this->currentCPUTemperature);
}

uint64_t SourceData::getCurrentCPUTemperatureTimestamp(void) const
{
    return (this->currentCPUTemperatureTimestamp);
}

bool SourceData::changedCPUTemperature(uint64_t fromTimestamp) const
{
    return (fromTimestamp != this->currentCPUTemperatureTimestamp);
}

bool SourceData::setCurrentCPUTemperature(float value, uint64_t timestamp)
{
    if (value != this->currentCPUTemperature)
    {
        if (value >= this->minCPUTemperature && value <= this->maxCPUTemperature)
        {
            this->currentCPUTemperature = value;
            this->currentCPUTemperatureTimestamp = timestamp;
            return (true);
        }
        else
        {
            return (false);
        }
    }
    else
    {
        this->currentCPUTemperatureTimestamp = timestamp;
        return (true);
    }
}

uint64_t SourceData::getNetworkDownloadBandwidthLimit(void) const
{
    return (this->totalNetworkDownloadBandwidthLimit);
}

bool SourceData::setNetworkDownloadBandwidthLimit(uint64_t bytes)
{
    this->totalNetworkDownloadBandwidthLimit = bytes;
    return (true);
}

uint64_t SourceData::getNetworkDownloadSpeed(void) const
{
    return (this->currentNetworkDownloadSpeed);
}

uint64_t SourceData::getNetworkDownloadSpeedTimestamp(void) const
{
    return (this->currentTotalNetworkDownloadedTimestamp);
}

bool SourceData::changedNetworkDownloadSpeed(uint64_t fromTimestamp) const
{
    return (fromTimestamp != this->currentTotalNetworkDownloadedTimestamp);
}

bool SourceData::setCurrentTotalNetworkDownloaded(uint64_t value, uint64_t timestamp)
{
    if (value != this->currentTotalNetworkDownloaded)
    {
        this->previousTotalNetworkDownloaded = this->currentTotalNetworkDownloaded;
        this->currentTotalNetworkDownloaded = value;
        this->currentCPUTemperatureTimestamp = timestamp;
        uint64_t diffBytes = this->currentTotalNetworkDownloaded - this->previousTotalNetworkDownloaded;
        uint16_t diffSeconds = (this->currentTotalNetworkDownloadedTimestamp - this->previousTotalNetworkDownloadedTimestamp) / 1000;
        this->currentNetworkDownloadSpeed = diffBytes / diffSeconds;
        return (true);
    }
    else
    {
        this->currentCPUTemperatureTimestamp = timestamp;
        uint64_t diffBytes = this->currentTotalNetworkDownloaded - this->previousTotalNetworkDownloaded;
        uint16_t diffSeconds = (this->currentTotalNetworkDownloadedTimestamp - this->previousTotalNetworkDownloadedTimestamp) / 1000;
        this->currentNetworkDownloadSpeed = diffBytes / diffSeconds;
        return (true);
    }
}
