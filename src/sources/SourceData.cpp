
#include "SourceData.hpp"
#include <Arduino.h>
#include <cstdio>
#include <cstring>
#include "../utils/Format.hpp"

SourceData::SourceData(bool truncateOverflows, uint64_t totalNetworkDownloadBandwidthLimit, uint64_t totalNetworkUploadBandwidthLimit)
{
    uint64_t currentTimestamp = millis();
    this->truncateOverflows = truncateOverflows;
    this->cpuLoadQueue = xQueueCreate(1, sizeof(SourceDataQueueCPULoadValue));
    this->usedMemoryQueue = xQueueCreate(1, sizeof(SourceDataQueueUsedMemoryValue));
    this->cpuTemperatureQueue = xQueueCreate(1, sizeof(SourceDataQueueCPUTemperatureValue));
    this->systemUptimeQueue = xQueueCreate(1, sizeof(SourceDataQueueUptimeValue));
    this->networkingDownloadQueue = xQueueCreate(1, sizeof(SourceDataQueueNetworkingValue));
    this->networkingUploadQueue = xQueueCreate(1, sizeof(SourceDataQueueNetworkingValue));
    this->networkingLimitsQueue = xQueueCreate(1, sizeof(SourceDataQueueNetworkingLimitsValue));
    this->fixedNetworkingLimits = totalNetworkDownloadBandwidthLimit > 0 && totalNetworkUploadBandwidthLimit > 0;
    this->setNetworkLimits(totalNetworkDownloadBandwidthLimit, totalNetworkUploadBandwidthLimit);
}

SourceData::~SourceData()
{
    vQueueDelete(this->cpuLoadQueue);
    vQueueDelete(this->usedMemoryQueue);
    vQueueDelete(this->cpuTemperatureQueue);
    vQueueDelete(this->systemUptimeQueue);
    vQueueDelete(this->networkingDownloadQueue);
    vQueueDelete(this->networkingUploadQueue);
    vQueueDelete(this->networkingLimitsQueue);
}

bool SourceData::hasFixedNetworkingLimits(void)
{
    return (this->fixedNetworkingLimits);
}

bool SourceData::setHostname(const char *hostname)
{
    if (hostname && strlen(hostname) > 0 && strlen(hostname) <= SourceData::MAX_HOSTNAME_LENGTH)
    {
        strncpy(this->hostname, hostname, sizeof(this->hostname));
        return (true);
    }
    else
    {
        return (false);
    }
}

void SourceData::getHostname(char *hostname, size_t count)
{
    if (this->hostname && strlen(this->hostname) > 0)
    {
        strncpy(hostname, this->hostname, count);
    }
    else
    {
        hostname[0] = '\0';
    }
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

// NET COMMON

SourceDataQueueNetworkingLimitsValue SourceData::getNetworkLimits(void)
{
    SourceDataQueueNetworkingLimitsValue data = {0, 0};
    if (this->networkingLimitsQueue != NULL)
    {
        if (xQueuePeek(this->networkingLimitsQueue, &data, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS)
        {
            data.byteDownloadLimit = 0;
            data.byteUploadLimit = 0;
        }
    }
    else
    {
        data.byteDownloadLimit = 0;
        data.byteUploadLimit = 0;
    }
    return (data);
}

bool SourceData::setNetworkLimits(uint64_t byteDownloadLimit, uint64_t byteUploadLimit)
{
    if (this->networkingLimitsQueue != NULL)
    {
        SourceDataQueueUptimeValue data = {byteDownloadLimit, byteUploadLimit};
        return (xQueueOverwrite(this->networkingLimitsQueue, &data) == pdPASS);
    }
    else
    {
        return (false);
    }
}

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
