
#include "SourceData.hpp"
#include <Arduino.h>
#include <cstdio>
#include "../utils/Format.hpp"

SourceData::SourceData(bool truncateOverflows, const char *networkInterfaceId, uint64_t totalNetworkDownloadBandwidthLimit, uint64_t totalNetworkUploadBandwidthLimit)
{
    uint64_t currentTimestamp = millis();
    this->cpuLoadQueue = xQueueCreate(1, sizeof(SourceDataQueueCPULoadValue));
    this->usedMemoryQueue = xQueueCreate(1, sizeof(SourceDataQueueUsedMemoryValue));
    this->cpuTemperatureQueue = xQueueCreate(1, sizeof(SourceDataQueueCPUTemperatureValue));
    this->systemUptimeQueue = xQueueCreate(1, sizeof(SourceDataQueueUptimeValue));
    this->networkingDownloadQueue = xQueueCreate(1, sizeof(SourceDataQueueNetworkingValue));
    this->networkingUploadQueue = xQueueCreate(1, sizeof(SourceDataQueueNetworkingValue));
    this->truncateOverflows = truncateOverflows;
    // this->setCurrentNetworkDownload(0, totalNetworkDownloadBandwidthLimit, currentTimestamp);
    // this->setCurrentNetworkUpload(0, totalNetworkUploadBandwidthLimit, currentTimestamp);
    //  this->totalNetworkDownloadBandwidthLimit = totalNetworkDownloadBandwidthLimit;
    //  this->totalNetworkUploadBandwidthLimit = totalNetworkUploadBandwidthLimit;
}

SourceData::~SourceData()
{
    vQueueDelete(this->cpuLoadQueue);
    vQueueDelete(this->usedMemoryQueue);
    vQueueDelete(this->cpuTemperatureQueue);
    vQueueDelete(this->systemUptimeQueue);
    vQueueDelete(this->networkingDownloadQueue);
    vQueueDelete(this->networkingUploadQueue);
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

SourceDataQueueUptimeValue SourceData::getCurrentUptime(void)
{
    SourceDataQueueUptimeValue data = {0, 0};
    if (this->systemUptimeQueue != NULL)
    {
        if (xQueuePeek(this->systemUptimeQueue, &data, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS)
        {
            data.seconds = 0;
            data.timestamp = 0;
        }
    }
    else
    {
        data.seconds = 0;
        data.timestamp = 0;
    }
    return (data);
}

bool SourceData::setCurrentUptime(uint64_t seconds, uint64_t timestamp)
{
    if (this->systemUptimeQueue != NULL)
    {
        SourceDataQueueUptimeValue data = this->getCurrentUptime();
        if (seconds != data.seconds)
        {
            data.seconds = seconds;
            data.timestamp = timestamp;
            return (xQueueOverwrite(this->systemUptimeQueue, &data) == pdPASS);
        }
        else
        {
            data.timestamp = timestamp;
            return (xQueueOverwrite(this->systemUptimeQueue, &data) == pdPASS);
        }
    }
    else
    {
        return (false);
    }
}

// NET DOWNLOAD BANDWIDTH

SourceDataQueueNetworkingValue SourceData::getCurrentNetworkDownload(void)
{
    SourceDataQueueNetworkingValue data = {0, 0, 0};
    if (this->networkingDownloadQueue != NULL)
    {
        if (xQueuePeek(this->networkingDownloadQueue, &data, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS)
        {
            data.totalBytesTransfered = 0;
            data.currentBandwidthBytesPerSecond = 0;
            data.timestamp = 0;
        }
    }
    else
    {
        data.totalBytesTransfered = 0;
        data.currentBandwidthBytesPerSecond = 0;
        data.timestamp = 0;
    }
    return (data);
}

bool SourceData::setCurrentNetworkDownload(uint64_t totalBytes, uint64_t timestamp)
{
    if (this->networkingDownloadQueue)
    {
        SourceDataQueueNetworkingValue data = this->getCurrentNetworkDownload();
        if (totalBytes != data.totalBytesTransfered)
        {
            uint64_t diffBytes = totalBytes - data.totalBytesTransfered;
            float diffSeconds = (timestamp - data.timestamp) / 1000.0;
            data.currentBandwidthBytesPerSecond = diffSeconds > 0 ? diffBytes / diffSeconds : 0;
            data.totalBytesTransfered = totalBytes;
            data.timestamp = timestamp;
            return (xQueueOverwrite(this->networkingDownloadQueue, &data) == pdPASS);
        }
        else if (timestamp != data.timestamp)
        {
            data.timestamp = timestamp;
            return (xQueueOverwrite(this->networkingDownloadQueue, &data) == pdPASS);
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}

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

SourceDataQueueNetworkingValue SourceData::getCurrentNetworkUpload(void)
{
    SourceDataQueueNetworkingValue data = {0, 0, 0};
    if (this->networkingUploadQueue != NULL)
    {
        if (xQueuePeek(this->networkingUploadQueue, &data, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS)
        {
            data.totalBytesTransfered = 0;
            data.currentBandwidthBytesPerSecond = 0;
            data.timestamp = 0;
        }
    }
    else
    {
        data.totalBytesTransfered = 0;
        data.currentBandwidthBytesPerSecond = 0;
        data.timestamp = 0;
    }
    return (data);
}

bool SourceData::setCurrentNetworkUpload(uint64_t totalBytes, uint64_t timestamp)
{
    if (this->networkingUploadQueue)
    {
        SourceDataQueueNetworkingValue data = this->getCurrentNetworkUpload();
        if (totalBytes != data.totalBytesTransfered)
        {
            uint64_t diffBytes = totalBytes - data.totalBytesTransfered;
            float diffSeconds = (timestamp - data.timestamp) / 1000.0;
            data.currentBandwidthBytesPerSecond = diffSeconds > 0 ? diffBytes / diffSeconds : 0;
            data.totalBytesTransfered = totalBytes;
            data.timestamp = timestamp;
            return (xQueueOverwrite(this->networkingUploadQueue, &data) == pdPASS);
        }
        else if (timestamp != data.timestamp)
        {
            data.timestamp = timestamp;
            return (xQueueOverwrite(this->networkingUploadQueue, &data) == pdPASS);
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}

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
