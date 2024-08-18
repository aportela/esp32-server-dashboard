
#include "SourceData.hpp"
#include <Arduino.h>
#include <cstdio>
#include "../utils/Format.hpp"

SourceData::SourceData(bool truncateOverflows, uint64_t totalNetworkDownloadBandwidthLimit, uint64_t totalNetworkUploadBandwidthLimit, const char *networkInterfaceId, const char *networkInterfaceName)
{

    this->cpuLoadQueue = xQueueCreate(1, sizeof(SourceDataQueueCPULoadValue));
    this->usedMemoryQueue = xQueueCreate(1, sizeof(SourceDataQueueUsedMemoryValue));
    this->cpuTemperatureQueue = xQueueCreate(1, sizeof(SourceDataQueueCPUTemperatureValue));
    this->truncateOverflows = truncateOverflows;
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
    vQueueDelete(this->usedMemoryQueue);
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

SourceDataQueueCPULoadValue SourceData::getCurrentCPULoad(void)
{
    SourceDataQueueCPULoadValue data = {0.0f, 0};
    if (this->cpuLoadQueue != NULL)
    {
        if (xQueuePeek(this->cpuLoadQueue, &data, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS)
        {
            data.loadPercent = 0.0f;
            data.timestamp = 0;
        }
    }
    else
    {
        data.loadPercent = 0.0f;
        data.timestamp = 0;
    }
    return (data);
}

bool SourceData::setCurrentCPULoad(float loadPercent, uint64_t timestamp)
{
    if (this->cpuLoadQueue != NULL)
    {
        SourceDataQueueCPULoadValue data = this->getCurrentCPULoad();
        if (loadPercent != data.loadPercent)
        {
            if (loadPercent >= MIN_CPU_LOAD && loadPercent <= MAX_CPU_LOAD)
            {
                data.loadPercent = loadPercent;
                data.timestamp = timestamp;
                return (xQueueOverwrite(this->cpuLoadQueue, &data) == pdPASS);
            }
            else if (truncateOverflows)
            {
                if (loadPercent < MIN_CPU_LOAD)
                {
                    data.loadPercent = MIN_CPU_LOAD;
                    data.timestamp = timestamp;
                    return (xQueueOverwrite(this->cpuLoadQueue, &data) == pdPASS);
                }
                else if (loadPercent > MAX_CPU_LOAD)
                {
                    data.loadPercent = MAX_CPU_LOAD;
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

SourceDataQueueUsedMemoryValue SourceData::getCurrentUsedMemory(void)
{
    SourceDataQueueUsedMemoryValue data = {0, 0, 0};
    if (this->usedMemoryQueue != NULL)
    {
        if (xQueuePeek(this->usedMemoryQueue, &data, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS)
        {
            data.usedBytes = 0;
            data.totalBytes = 0;
            data.timestamp = 0;
        }
    }
    else
    {
        data.usedBytes = 0;
        data.totalBytes = 0;
        data.timestamp = 0;
    }
    return (data);
}

bool SourceData::setCurrentUsedMemory(uint64_t usedBytes, uint64_t totalBytes, uint64_t timestamp)
{
    if (this->usedMemoryQueue)
    {
        SourceDataQueueUsedMemoryValue data = this->getCurrentUsedMemory();
        if (totalBytes != data.totalBytes)
        {
            data.totalBytes = totalBytes;
            data.timestamp = timestamp;
        }
        if (usedBytes != data.usedBytes)
        {
            if (usedBytes >= 0 && usedBytes <= data.totalBytes)
            {
                data.usedBytes = usedBytes;
                data.timestamp = timestamp;
                return (xQueueOverwrite(this->usedMemoryQueue, &data) == pdPASS);
            }
            else if (truncateOverflows)
            {
                if (usedBytes < 0)
                {
                    data.usedBytes = 0;
                    data.timestamp = timestamp;
                    return (xQueueOverwrite(this->usedMemoryQueue, &data) == pdPASS);
                }
                else if (usedBytes > data.totalBytes)
                {
                    data.usedBytes = data.totalBytes;
                    data.timestamp = timestamp;
                    return (xQueueOverwrite(this->usedMemoryQueue, &data) == pdPASS);
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
            return (xQueueOverwrite(this->usedMemoryQueue, &data) == pdPASS);
        }
    }
    else
    {
        return (false);
    }
}

// CPU TEMPERATURE

SourceDataQueueCPUTemperatureValue SourceData::getCurrentCPUTemperature(void)
{
    SourceDataQueueCPUTemperatureValue data = {0.0f, 0};
    if (this->cpuTemperatureQueue != NULL)
    {
        if (xQueuePeek(this->cpuTemperatureQueue, &data, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS)
        {
            data.celsious = 0.0f;
            data.timestamp = 0;
        }
    }
    else
    {
        data.celsious = 0.0f;
        data.timestamp = 0;
    }
    return (data);
}

bool SourceData::setCurrentCPUTemperature(float celsious, uint64_t timestamp)
{
    if (this->cpuTemperatureQueue != NULL)
    {
        SourceDataQueueCPUTemperatureValue data = this->getCurrentCPUTemperature();
        if (celsious != data.celsious)
        {
            if (celsious >= MIN_CPU_TEMPERATURE && celsious <= MAX_CPU_TEMPERATURE)
            {
                data.celsious = celsious;
                data.timestamp = timestamp;
                return (xQueueOverwrite(this->cpuTemperatureQueue, &data) == pdPASS);
            }
            else if (truncateOverflows)
            {
                if (celsious < MIN_CPU_TEMPERATURE)
                {
                    data.celsious = MIN_CPU_TEMPERATURE;
                    data.timestamp = timestamp;
                    return (xQueueOverwrite(this->cpuTemperatureQueue, &data) == pdPASS);
                }
                else if (celsious > MAX_CPU_TEMPERATURE)
                {
                    data.celsious = MAX_CPU_TEMPERATURE;
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
