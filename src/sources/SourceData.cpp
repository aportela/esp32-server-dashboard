
#include "SourceData.hpp"
#include <Arduino.h>
#include <cstdio>
#include "../utils/Format.hpp"

#define MIN_CPU_LOAD 0
#define MAX_CPU_LOAD 100

SourceData::SourceData(bool truncateOverflows, float minCPUTemperature, float maxCPUTemperature, uint64_t totalNetworkDownloadBandwidthLimit, uint64_t totalNetworkUploadBandwidthLimit, const char *networkInterfaceId, const char *networkInterfaceName)
{
    this->mutex = xSemaphoreCreateMutex();
    this->truncateOverflows = truncateOverflows;
    this->totalMemory = totalMemory;
    this->minCPUTemperature = minCPUTemperature;
    this->maxCPUTemperature = maxCPUTemperature;
    this->totalNetworkDownloadBandwidthLimit = totalNetworkDownloadBandwidthLimit;
    this->totalNetworkUploadBandwidthLimit = totalNetworkUploadBandwidthLimit;
    if (networkInterfaceId != NULL && strlen(networkInterfaceId) > 0)
    {
        strncpy(this->networkInterfaceId, networkInterfaceId, sizeof(this->networkInterfaceId));
    }
    if (networkInterfaceName != NULL && strlen(networkInterfaceName) > 0)
    {
        strncpy(this->networkInterfaceName, networkInterfaceName, sizeof(this->networkInterfaceName));
    }
}

SourceData::~SourceData()
{
    if (this->mutex != NULL)
    {
        vSemaphoreDelete(this->mutex);
    }
}

// NET IFACE

void SourceData::getNetworkInterfaceId(char *buffer, size_t buffer_count)
{
    strncpy(buffer, this->networkInterfaceId, buffer_count);
}

void SourceData::getNetworkInterfaceName(char *buffer, size_t buffer_count)
{
    strncpy(buffer, this->networkInterfaceName, buffer_count);
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
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    float load = this->currentCPULoad;
    xSemaphoreGive(this->mutex);
    return (load);
}

uint64_t SourceData::getCurrentCPULoadTimestamp(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t timestamp = this->currentCPULoadTimestamp;
    xSemaphoreGive(this->mutex);
    return (timestamp);
}

bool SourceData::changedCPULoad(uint64_t fromTimestamp) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t timestamp = this->currentCPULoadTimestamp;
    xSemaphoreGive(this->mutex);
    return (fromTimestamp != timestamp);
}

bool SourceData::setCurrentCPULoad(float value, uint64_t timestamp)
{
    if (value != this->currentCPULoad)
    {
        if (value >= MIN_CPU_LOAD && value <= MAX_CPU_LOAD)
        {
            xSemaphoreTake(this->mutex, portMAX_DELAY);
            this->currentCPULoad = value;
            this->currentCPULoadTimestamp = timestamp;
            xSemaphoreGive(this->mutex);
            return (true);
        }
        else if (truncateOverflows)
        {
            if (value < MIN_CPU_LOAD)
            {
                xSemaphoreTake(this->mutex, portMAX_DELAY);
                this->currentCPULoad = MIN_CPU_LOAD;
                this->currentCPULoadTimestamp = timestamp;
                xSemaphoreGive(this->mutex);
                return (true);
            }
            else if (value > MAX_CPU_LOAD)
            {
                xSemaphoreTake(this->mutex, portMAX_DELAY);
                this->currentCPULoad = MAX_CPU_LOAD;
                this->currentCPULoadTimestamp = timestamp;
                xSemaphoreGive(this->mutex);
                return (true);
            }
        }
        else
        {
            return (false);
        }
    }
    else
    {
        xSemaphoreTake(this->mutex, portMAX_DELAY);
        this->currentCPULoadTimestamp = timestamp;
        xSemaphoreGive(this->mutex);
        return (true);
    }
}

// MEMORY

uint64_t SourceData::getTotalMemory(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t memory = this->totalMemory;
    xSemaphoreGive(this->mutex);
    return (memory);
}

uint64_t SourceData::getUsedMemory(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t memory = this->usedMemory;
    xSemaphoreGive(this->mutex);
    return (memory);
}

uint64_t SourceData::getCurrentUsedMemoryTimestamp(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t timestamp = this->currentUsedMemoryTimestamp;
    xSemaphoreGive(this->mutex);
    return (timestamp);
}

bool SourceData::changedUsedMemory(uint64_t fromTimestamp) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t timestamp = this->currentUsedMemoryTimestamp;
    xSemaphoreGive(this->mutex);
    return (fromTimestamp != timestamp);
}

bool SourceData::setTotalMemory(uint64_t bytes)
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    this->totalMemory = bytes;
    xSemaphoreGive(this->mutex);
}

bool SourceData::setUsedMemory(uint64_t bytes, uint64_t timestamp)
{
    if (bytes != this->usedMemory)
    {
        if (bytes >= 0 && bytes <= this->totalMemory)
        {
            xSemaphoreTake(this->mutex, portMAX_DELAY);
            this->usedMemory = bytes;
            this->currentUsedMemoryTimestamp = timestamp;
            xSemaphoreGive(this->mutex);
            return (true);
        }
        else if (truncateOverflows)
        {
            if (bytes < 0)
            {
                xSemaphoreTake(this->mutex, portMAX_DELAY);
                this->usedMemory = 0;
                this->currentUsedMemoryTimestamp = timestamp;
                xSemaphoreGive(this->mutex);
                return (true);
            }
            else if (bytes > this->totalMemory)
            {
                xSemaphoreTake(this->mutex, portMAX_DELAY);
                this->usedMemory = this->totalMemory;
                this->currentUsedMemoryTimestamp = timestamp;
                xSemaphoreGive(this->mutex);
                return (true);
            }
        }
        else
        {
            return (false);
        }
    }
    else
    {
        xSemaphoreTake(this->mutex, portMAX_DELAY);
        this->currentUsedMemoryTimestamp = timestamp;
        xSemaphoreGive(this->mutex);
        return (true);
    }
}

bool SourceData::setUsedAndTotalMemory(uint64_t usedBytes, uint64_t totalBytes, uint64_t timestamp)
{
    if (totalBytes != this->totalMemory)
    {
        xSemaphoreTake(this->mutex, portMAX_DELAY);
        this->totalMemory = totalBytes;
        xSemaphoreGive(this->mutex);
    }
    if (usedBytes != this->usedMemory)
    {
        if (usedBytes >= 0 && usedBytes <= this->totalMemory)
        {
            xSemaphoreTake(this->mutex, portMAX_DELAY);
            this->usedMemory = usedBytes;
            this->currentUsedMemoryTimestamp = timestamp;
            xSemaphoreGive(this->mutex);
            return (true);
        }
        else if (truncateOverflows)
        {
            if (usedBytes < 0)
            {
                xSemaphoreTake(this->mutex, portMAX_DELAY);
                this->usedMemory = 0;
                this->currentUsedMemoryTimestamp = timestamp;
                xSemaphoreGive(this->mutex);
                return (true);
            }
            else if (usedBytes > this->totalMemory)
            {
                xSemaphoreTake(this->mutex, portMAX_DELAY);
                this->usedMemory = this->totalMemory;
                this->currentUsedMemoryTimestamp = timestamp;
                xSemaphoreGive(this->mutex);
                return (true);
            }
        }
        else
        {
            return (false);
        }
    }
    else
    {
        xSemaphoreTake(this->mutex, portMAX_DELAY);
        this->currentUsedMemoryTimestamp = timestamp;
        xSemaphoreGive(this->mutex);
        return (true);
    }
}

// CPU TEMPERATURE

float SourceData::getMinCPUTemperature(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    float temperature = this->minCPUTemperature;
    xSemaphoreGive(this->mutex);
    return (temperature);
}

bool SourceData::setMinCPUTemperature(float celsious)
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    this->minCPUTemperature = celsious;
    xSemaphoreGive(this->mutex);
    return (true);
}

float SourceData::getMaxCPUTemperature(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    float temperature = this->maxCPUTemperature;
    xSemaphoreGive(this->mutex);
    return (temperature);
}

bool SourceData::setMaxCPUTemperature(float celsious)
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    this->maxCPUTemperature = celsious;
    xSemaphoreGive(this->mutex);
    return (true);
}

float SourceData::getCurrentCPUTemperature(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    float temperature = this->currentCPUTemperature;
    xSemaphoreGive(this->mutex);
    return (temperature);
}

uint64_t SourceData::getCurrentCPUTemperatureTimestamp(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t timestamp = this->currentCPUTemperatureTimestamp;
    xSemaphoreGive(this->mutex);
    return (timestamp);
}

bool SourceData::changedCPUTemperature(uint64_t fromTimestamp) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t timestamp = this->currentCPUTemperatureTimestamp;
    xSemaphoreGive(this->mutex);
    return (fromTimestamp != timestamp);
}

bool SourceData::setCurrentCPUTemperature(float celsious, uint64_t timestamp)
{
    if (celsious != this->currentCPUTemperature)
    {
        if (celsious >= this->minCPUTemperature && celsious <= this->maxCPUTemperature)
        {
            xSemaphoreTake(this->mutex, portMAX_DELAY);
            this->currentCPUTemperature = celsious;
            this->currentCPUTemperatureTimestamp = timestamp;
            xSemaphoreGive(this->mutex);
            return (true);
        }
        else if (truncateOverflows)
        {
            if (celsious < this->minCPUTemperature)
            {
                xSemaphoreTake(this->mutex, portMAX_DELAY);
                this->currentCPUTemperature = this->minCPUTemperature;
                this->currentCPUTemperatureTimestamp = timestamp;
                xSemaphoreGive(this->mutex);
                return (true);
            }
            else if (celsious > this->maxCPUTemperature)
            {
                xSemaphoreTake(this->mutex, portMAX_DELAY);
                this->currentCPUTemperature = this->maxCPUTemperature;
                this->currentCPUTemperatureTimestamp = timestamp;
                xSemaphoreGive(this->mutex);
                return (true);
            }
        }
        else
        {
            return (false);
        }
    }
    else
    {
        xSemaphoreTake(this->mutex, portMAX_DELAY);
        this->currentCPUTemperatureTimestamp = timestamp;
        xSemaphoreGive(this->mutex);
        return (true);
    }
}

// SYSTEM

uint64_t SourceData::getCurrentUptimeSeconds(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t seconds = this->currentUptimeSeconds;
    xSemaphoreGive(this->mutex);
    return (seconds);
}

uint64_t SourceData::getCurrentUptimeSecondsTimestamp(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t timestamp = this->currentUptimeSecondsTimestamp;
    xSemaphoreGive(this->mutex);
    return (timestamp);
}

bool SourceData::changedUptimeSeconds(uint64_t fromTimestamp) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t timestamp = this->currentUptimeSecondsTimestamp;
    xSemaphoreGive(this->mutex);
    return (fromTimestamp != timestamp);
}

bool SourceData::setCurrentUptimeSeconds(uint64_t seconds, uint64_t timestamp)
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    if (seconds != this->currentUptimeSeconds)
    {
        this->currentUptimeSeconds = seconds;
    }
    this->currentUptimeSecondsTimestamp = timestamp;
    xSemaphoreGive(this->mutex);
    return (true);
}

// NET DOWNLOAD BANDWIDTH

uint64_t SourceData::getNetworkDownloadBandwidthLimit(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t bytes = this->totalNetworkDownloadBandwidthLimit;
    xSemaphoreGive(this->mutex);
    return (bytes);
}

bool SourceData::setNetworkDownloadBandwidthLimit(uint64_t bytes)
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    this->totalNetworkDownloadBandwidthLimit = bytes;
    xSemaphoreGive(this->mutex);
    return (true);
}

uint64_t SourceData::getNetworkDownloadSpeed(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t bytesPerSecond = this->currentNetworkDownloadSpeed;
    xSemaphoreGive(this->mutex);
    return (bytesPerSecond);
}

uint64_t SourceData::getNetworkDownloadSpeedTimestamp(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t timestamp = this->currentTotalNetworkDownloadedTimestamp;
    xSemaphoreGive(this->mutex);
    return (timestamp);
}

uint64_t SourceData::getCurrentTotalNetworkDownloaded(void)
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t bytesPerSecond = this->currentTotalNetworkDownloaded;
    xSemaphoreGive(this->mutex);
    return (bytesPerSecond);
}

bool SourceData::changedNetworkDownloadSpeed(uint64_t fromTimestamp) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t timestamp = this->currentTotalNetworkDownloadedTimestamp;
    xSemaphoreGive(this->mutex);
    return (fromTimestamp != timestamp);
}

bool SourceData::setCurrentTotalNetworkDownloaded(uint64_t bytes, uint64_t timestamp)
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    if (bytes != this->currentTotalNetworkDownloaded)
    {
        this->previousTotalNetworkDownloaded = this->currentTotalNetworkDownloaded;
        this->currentTotalNetworkDownloaded = bytes;
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
    xSemaphoreGive(this->mutex);
    return (true);
}

// NET UPLOAD BANDWIDTH

uint64_t SourceData::getNetworkUploadBandwidthLimit(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t bytes = this->totalNetworkUploadBandwidthLimit;
    xSemaphoreGive(this->mutex);
    return (bytes);
}

bool SourceData::setNetworkUploadBandwidthLimit(uint64_t bytes)
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    this->totalNetworkUploadBandwidthLimit = bytes;
    xSemaphoreGive(this->mutex);
    return (true);
}

uint64_t SourceData::getNetworkUploadSpeed(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t bytesPerSecond = this->currentNetworkUploadSpeed;
    xSemaphoreGive(this->mutex);
    return (bytesPerSecond);
}

uint64_t SourceData::getNetworkUploadSpeedTimestamp(void) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t timestamp = this->currentTotalNetworkUploadedTimestamp;
    xSemaphoreGive(this->mutex);
    return (timestamp);
}

uint64_t SourceData::getCurrentTotalNetworkUploaded(void)
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t bytesPerSecond = this->currentTotalNetworkUploaded;
    xSemaphoreGive(this->mutex);
    return (bytesPerSecond);
}

bool SourceData::changedNetworkUploadSpeed(uint64_t fromTimestamp) const
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    uint64_t timestamp = this->currentTotalNetworkUploadedTimestamp;
    xSemaphoreGive(this->mutex);
    return (fromTimestamp != timestamp);
}

bool SourceData::setCurrentTotalNetworkUploaded(uint64_t bytes, uint64_t timestamp)
{
    xSemaphoreTake(this->mutex, portMAX_DELAY);
    if (bytes != this->currentTotalNetworkUploaded)
    {
        this->previousTotalNetworkUploaded = this->currentTotalNetworkUploaded;
        this->currentTotalNetworkUploaded = bytes;
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
    xSemaphoreGive(this->mutex);
    return (true);
}
