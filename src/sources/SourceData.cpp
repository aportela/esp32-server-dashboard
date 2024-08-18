
#include "SourceData.hpp"
#include <Arduino.h>
#include <cstdio>
#include "../utils/Format.hpp"

#define MIN_CPU_LOAD 0
#define MAX_CPU_LOAD 100

SourceData::SourceData(bool truncateOverflows, float minCPUTemperature, float maxCPUTemperature, uint64_t totalNetworkDownloadBandwidthLimit, uint64_t totalNetworkUploadBandwidthLimit, const char *networkInterfaceId, const char *networkInterfaceName)
{

    this->cpuLoadQueue = xQueueCreate(1, sizeof(SourceDataQueueCPULoadValue));
    this->cpuTemperatureQueue = xQueueCreate(1, sizeof(SourceDataQueueCPUTemperatureValue));
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
    vQueueDelete(this->cpuLoadQueue);
    vQueueDelete(this->cpuTemperatureQueue);
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

SourceDataQueueCPULoadValue SourceData::getCurrentCPULoad(void)
{
    SourceDataQueueCPULoadValue data = {0.0f, 0};
    if (this->cpuLoadQueue != NULL)
    {
        if (xQueuePeek(this->cpuLoadQueue, &data, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS)
        {
            data.value = 0.0f;
            data.timestamp = 0;
        }
    }
    else
    {
        data.value = 0.0f;
        data.timestamp = 0;
    }
    return (data);
}

bool SourceData::setCurrentCPULoad(float value, uint64_t timestamp)
{
    if (this->cpuLoadQueue != NULL)
    {
        SourceDataQueueCPULoadValue data = this->getCurrentCPULoad();
        if (value != data.value)
        {
            if (value >= MIN_CPU_LOAD && value <= MAX_CPU_LOAD)
            {
                data.value = value;
                data.timestamp = timestamp;
                return (xQueueOverwrite(this->cpuLoadQueue, &data) == pdPASS);
            }
            else if (truncateOverflows)
            {
                if (value < MIN_CPU_LOAD)
                {
                    data.value = MIN_CPU_LOAD;
                    data.timestamp = timestamp;
                    return (xQueueOverwrite(this->cpuLoadQueue, &data) == pdPASS);
                }
                else if (value > MAX_CPU_LOAD)
                {
                    data.value = MAX_CPU_LOAD;
                    data.timestamp = timestamp;
                    return (xQueueOverwrite(this->cpuLoadQueue, &data) == pdPASS);
                }
            }
            else
            {
                return (false);
            }
        }
        else
        {
            data.timestamp = timestamp;
            return (xQueueOverwrite(this->cpuLoadQueue, &data) == pdPASS);
        }
    }
    else
    {
        return (false);
    }
}

// MEMORY

uint64_t SourceData::getTotalMemory(void) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t memory = this->totalMemory;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (memory);
}

uint64_t SourceData::getUsedMemory(void) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t memory = this->usedMemory;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (memory);
}

uint64_t SourceData::getCurrentUsedMemoryTimestamp(void) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t timestamp = this->currentUsedMemoryTimestamp;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (timestamp);
}

bool SourceData::changedUsedMemory(uint64_t fromTimestamp) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t timestamp = this->currentUsedMemoryTimestamp;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (fromTimestamp != timestamp);
}

bool SourceData::setTotalMemory(uint64_t bytes)
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    this->totalMemory = bytes;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (true);
}

bool SourceData::setUsedMemory(uint64_t bytes, uint64_t timestamp)
{
    if (bytes != this->usedMemory)
    {
        if (bytes >= 0 && bytes <= this->totalMemory)
        {
#ifdef USE_MUTEX
            xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
            this->usedMemory = bytes;
            this->currentUsedMemoryTimestamp = timestamp;
#ifdef USE_MUTEX
            xSemaphoreGive(this->mutex);
#endif
            return (true);
        }
        else if (truncateOverflows)
        {
            if (bytes < 0)
            {
#ifdef USE_MUTEX
                xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
                this->usedMemory = 0;
                this->currentUsedMemoryTimestamp = timestamp;
#ifdef USE_MUTEX
                xSemaphoreGive(this->mutex);
#endif
                return (true);
            }
            else if (bytes > this->totalMemory)
            {
#ifdef USE_MUTEX
                xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
                this->usedMemory = this->totalMemory;
                this->currentUsedMemoryTimestamp = timestamp;
#ifdef USE_MUTEX
                xSemaphoreGive(this->mutex);
#endif
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
#ifdef USE_MUTEX
        xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
        this->currentUsedMemoryTimestamp = timestamp;
#ifdef USE_MUTEX
        xSemaphoreGive(this->mutex);
#endif
        return (true);
    }
}

bool SourceData::setUsedAndTotalMemory(uint64_t usedBytes, uint64_t totalBytes, uint64_t timestamp)
{
    if (totalBytes != this->totalMemory)
    {
#ifdef USE_MUTEX
        xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
        this->totalMemory = totalBytes;
#ifdef USE_MUTEX
        xSemaphoreGive(this->mutex);
#endif
    }
    if (usedBytes != this->usedMemory)
    {
        if (usedBytes >= 0 && usedBytes <= this->totalMemory)
        {
#ifdef USE_MUTEX
            xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
            this->usedMemory = usedBytes;
            this->currentUsedMemoryTimestamp = timestamp;
#ifdef USE_MUTEX
            xSemaphoreGive(this->mutex);
#endif
            return (true);
        }
        else if (truncateOverflows)
        {
            if (usedBytes < 0)
            {
#ifdef USE_MUTEX
                xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
                this->usedMemory = 0;
                this->currentUsedMemoryTimestamp = timestamp;
#ifdef USE_MUTEX
                xSemaphoreGive(this->mutex);
#endif
                return (true);
            }
            else if (usedBytes > this->totalMemory)
            {
#ifdef USE_MUTEX
                xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
                this->usedMemory = this->totalMemory;
                this->currentUsedMemoryTimestamp = timestamp;
#ifdef USE_MUTEX
                xSemaphoreGive(this->mutex);
#endif
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
#ifdef USE_MUTEX
        xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
        this->currentUsedMemoryTimestamp = timestamp;
#ifdef USE_MUTEX
        xSemaphoreGive(this->mutex);
#endif
        return (true);
    }
}

// CPU TEMPERATURE

float SourceData::getMinCPUTemperature(void) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    float temperature = this->minCPUTemperature;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (temperature);
}

bool SourceData::setMinCPUTemperature(float celsious)
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    this->minCPUTemperature = celsious;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (true);
}

float SourceData::getMaxCPUTemperature(void) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    float temperature = this->maxCPUTemperature;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (temperature);
}

bool SourceData::setMaxCPUTemperature(float celsious)
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    this->maxCPUTemperature = celsious;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (true);
}

SourceDataQueueCPUTemperatureValue SourceData::getCurrentCPUTemperature(void)
{
    SourceDataQueueCPUTemperatureValue data = {0.0f, 0};
    if (this->cpuTemperatureQueue != NULL)
    {
        if (xQueuePeek(this->cpuTemperatureQueue, &data, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS)
        {
            data.value = 0.0f;
            data.timestamp = 0;
        }
    }
    else
    {
        data.value = 0.0f;
        data.timestamp = 0;
    }
    return (data);
}

bool SourceData::setCurrentCPUTemperature(float celsious, uint64_t timestamp)
{
    if (this->cpuTemperatureQueue != NULL)
    {
        SourceDataQueueCPUTemperatureValue data = this->getCurrentCPUTemperature();
        if (celsious != data.value)
        {
            if (celsious >= this->minCPUTemperature && celsious <= this->maxCPUTemperature)
            {
                data.value = celsious;
                data.timestamp = timestamp;
                return (xQueueOverwrite(this->cpuTemperatureQueue, &data) == pdPASS);
            }
            else if (truncateOverflows)
            {
                if (celsious < this->minCPUTemperature)
                {
                    data.value = this->minCPUTemperature;
                    data.timestamp = timestamp;
                    return (xQueueOverwrite(this->cpuTemperatureQueue, &data) == pdPASS);
                }
                else if (celsious > this->maxCPUTemperature)
                {
                    data.value = this->maxCPUTemperature;
                    data.timestamp = timestamp;
                    return (xQueueOverwrite(this->cpuTemperatureQueue, &data) == pdPASS);
                }
            }
            else
            {
                return (false);
            }
        }
        else
        {
            data.timestamp = timestamp;
            return (xQueueOverwrite(this->cpuTemperatureQueue, &data) == pdPASS);
        }
    }
    else
    {
        return (false);
    }
}

// SYSTEM

uint64_t SourceData::getCurrentUptimeSeconds(void) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t seconds = this->currentUptimeSeconds;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (seconds);
}

uint64_t SourceData::getCurrentUptimeSecondsTimestamp(void) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t timestamp = this->currentUptimeSecondsTimestamp;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (timestamp);
}

bool SourceData::changedUptimeSeconds(uint64_t fromTimestamp) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t timestamp = this->currentUptimeSecondsTimestamp;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (fromTimestamp != timestamp);
}

bool SourceData::setCurrentUptimeSeconds(uint64_t seconds, uint64_t timestamp)
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    if (seconds != this->currentUptimeSeconds)
    {
        this->currentUptimeSeconds = seconds;
    }
    this->currentUptimeSecondsTimestamp = timestamp;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (true);
}

// NET DOWNLOAD BANDWIDTH

uint64_t SourceData::getNetworkDownloadBandwidthLimit(void) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t bytes = this->totalNetworkDownloadBandwidthLimit;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (bytes);
}

bool SourceData::setNetworkDownloadBandwidthLimit(uint64_t bytes)
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    this->totalNetworkDownloadBandwidthLimit = bytes;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (true);
}

uint64_t SourceData::getNetworkDownloadSpeed(void) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t bytesPerSecond = this->currentNetworkDownloadSpeed;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (bytesPerSecond);
}

uint64_t SourceData::getNetworkDownloadSpeedTimestamp(void) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t timestamp = this->currentTotalNetworkDownloadedTimestamp;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (timestamp);
}

uint64_t SourceData::getCurrentTotalNetworkDownloaded(void)
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t bytesPerSecond = this->currentTotalNetworkDownloaded;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (bytesPerSecond);
}

bool SourceData::changedNetworkDownloadSpeed(uint64_t fromTimestamp) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t timestamp = this->currentTotalNetworkDownloadedTimestamp;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (fromTimestamp != timestamp);
}

bool SourceData::setCurrentTotalNetworkDownloaded(uint64_t bytes, uint64_t timestamp)
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
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
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (true);
}

// NET UPLOAD BANDWIDTH

uint64_t SourceData::getNetworkUploadBandwidthLimit(void) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t bytes = this->totalNetworkUploadBandwidthLimit;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (bytes);
}

bool SourceData::setNetworkUploadBandwidthLimit(uint64_t bytes)
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    this->totalNetworkUploadBandwidthLimit = bytes;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (true);
}

uint64_t SourceData::getNetworkUploadSpeed(void) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t bytesPerSecond = this->currentNetworkUploadSpeed;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (bytesPerSecond);
}

uint64_t SourceData::getNetworkUploadSpeedTimestamp(void) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t timestamp = this->currentTotalNetworkUploadedTimestamp;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (timestamp);
}

uint64_t SourceData::getCurrentTotalNetworkUploaded(void)
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t bytesPerSecond = this->currentTotalNetworkUploaded;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (bytesPerSecond);
}

bool SourceData::changedNetworkUploadSpeed(uint64_t fromTimestamp) const
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
    uint64_t timestamp = this->currentTotalNetworkUploadedTimestamp;
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (fromTimestamp != timestamp);
}

bool SourceData::setCurrentTotalNetworkUploaded(uint64_t bytes, uint64_t timestamp)
{
#ifdef USE_MUTEX
    xSemaphoreTake(this->mutex, portMAX_DELAY);
#endif
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
#ifdef USE_MUTEX
    xSemaphoreGive(this->mutex);
#endif
    return (true);
}
