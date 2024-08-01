
#include "SourceData.hpp"
#include <Arduino.h>
#include <cstdio>
#include "../utils/Format.hpp"

#define MIN_CPU_LOAD 0
#define MAX_CPU_LOAD 100

SourceData::SourceData(uint64_t totalMemory, float minCPUTemperature, float maxCPUTemperature, uint64_t totalNetworkDownloadBandwidthLimit, uint64_t totalNetworkUploadBandwidthLimit)
{
    this->totalMemory = totalMemory;
    this->minCPUTemperature = minCPUTemperature;
    this->maxCPUTemperature = maxCPUTemperature;
    this->totalNetworkDownloadBandwidthLimit = totalNetworkDownloadBandwidthLimit;
    this->totalNetworkUploadBandwidthLimit = totalNetworkUploadBandwidthLimit;
}

SourceData::~SourceData()
{
}

// CPU LOAD

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

// MEMORY

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

// CPU TEMPERATURE

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

// NET DOWNLOAD BANDWIDTH

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

uint64_t SourceData::getCurrentTotalNetworkDownloaded(void)
{
    return (this->currentTotalNetworkDownloaded);
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
    }
    else
    {
        this->previousTotalNetworkDownloaded = this->currentTotalNetworkDownloaded;
    }
    uint64_t diffBytes = this->currentTotalNetworkDownloaded - this->previousTotalNetworkDownloaded;
    this->previousTotalNetworkDownloadedTimestamp = this->currentTotalNetworkDownloadedTimestamp;
    this->currentTotalNetworkDownloadedTimestamp = timestamp;
    float diffSeconds = (this->currentTotalNetworkDownloadedTimestamp - this->previousTotalNetworkDownloadedTimestamp) / 1000.0;
    this->currentNetworkDownloadSpeed = diffSeconds > 0 ? diffBytes / diffSeconds : 0;
    return (true);
}

// NET UPLOAD BANDWIDTH

uint64_t SourceData::getNetworkUploadBandwidthLimit(void) const
{
    return (this->totalNetworkUploadBandwidthLimit);
}

bool SourceData::setNetworkUploadBandwidthLimit(uint64_t bytes)
{
    this->totalNetworkUploadBandwidthLimit = bytes;
    return (true);
}

uint64_t SourceData::getNetworkUploadSpeed(void) const
{
    return (this->currentNetworkUploadSpeed);
}

uint64_t SourceData::getNetworkUploadSpeedTimestamp(void) const
{
    return (this->currentTotalNetworkUploadedTimestamp);
}

uint64_t SourceData::getCurrentTotalNetworkUploaded(void)
{
    return (this->currentTotalNetworkUploaded);
}

bool SourceData::changedNetworkUploadSpeed(uint64_t fromTimestamp) const
{
    return (fromTimestamp != this->currentTotalNetworkUploadedTimestamp);
}

bool SourceData::setCurrentTotalNetworkUploaded(uint64_t value, uint64_t timestamp)
{
    if (value != this->currentTotalNetworkUploaded)
    {
        this->previousTotalNetworkUploaded = this->currentTotalNetworkUploaded;
        this->currentTotalNetworkUploaded = value;
    }
    else
    {
        this->previousTotalNetworkDownloaded = this->currentTotalNetworkUploaded;
    }
    uint64_t diffBytes = this->currentTotalNetworkUploaded - this->previousTotalNetworkUploaded;
    this->previousTotalNetworkUploadedTimestamp = this->currentTotalNetworkUploadedTimestamp;
    this->currentTotalNetworkUploadedTimestamp = timestamp;
    float diffSeconds = (this->currentTotalNetworkUploadedTimestamp - this->previousTotalNetworkUploadedTimestamp) / 1000.0;
    this->currentNetworkUploadSpeed = diffSeconds > 0 ? diffBytes / diffSeconds : 0;
    return (true);
}
