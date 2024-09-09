
#include "SourceData.hpp"
#include <Arduino.h>
#include <cstdio>
#include <cstring>
#include "../utils/Format.hpp"

#define MIN_CPU_USAGE_LIMIT_VALUE 0
#define MAX_CPU_USAGE_LIMIT_VALUE 100

SourceData::SourceData(bool truncateOverflows, uint64_t totalNetworkDownloadBandwidthLimit, uint64_t totalNetworkUploadBandwidthLimit)
{
    uint64_t currentTimestamp = millis();
    this->truncateOverflows = truncateOverflows;
    this->cpuLoadQueue = xQueueCreate(1, sizeof(SourceDataQueueCPUValues));
    this->usedMemoryQueue = xQueueCreate(1, sizeof(SourceDataQueueUsedMemoryValues));
    this->cpuTemperatureQueue = xQueueCreate(1, sizeof(SourceDataQueueCPUTemperatureValue));
    this->systemUptimeQueue = xQueueCreate(1, sizeof(SourceDataQueueUptimeValue));
    this->networkingDownloadQueue = xQueueCreate(1, sizeof(SourceDataQueueNetworkingValue));
    this->networkingUploadQueue = xQueueCreate(1, sizeof(SourceDataQueueNetworkingValue));
    this->networkingLimitsQueue = xQueueCreate(1, sizeof(SourceDataQueueNetworkingLimitsValue));
    this->fixedNetworkingLimits = totalNetworkDownloadBandwidthLimit > 0 && totalNetworkUploadBandwidthLimit > 0;
    this->SetNetworkLimits(totalNetworkDownloadBandwidthLimit, totalNetworkUploadBandwidthLimit);
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

bool SourceData::HasFixedNetworkingLimits(void)
{
    return (this->fixedNetworkingLimits);
}

bool SourceData::SetHostname(const char *hostname)
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

void SourceData::GetHostname(char *hostname, size_t count)
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

SourceDataQueueCPUValues SourceData::GetCurrentCPUData(void)
{
    SourceDataQueueCPUValues data = {0.0f, 0};
    if (this->cpuLoadQueue != NULL)
    {
        if (xQueuePeek(this->cpuLoadQueue, &data, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS)
        {
            data.loadPercent = 0.0f;
            data.usageSystem = 0.0f;
            data.usageUser = 0.0f;
            data.usageIdle = 0.0f;
            data.usageNice = 0.0f;
            data.usageIOWait = 0.0f;
            data.usageIRQ = 0.0f;
            data.usageSoftIRQ = 0.0f;
            data.usageGuest = 0.0f;
            data.usageGuestNice = 0.0f;
            data.usageSteal = 0.0f;
            data.timestamp = 0;
        }
    }
    else
    {
        data.loadPercent = 0.0f;
        data.usageSystem = 0.0f;
        data.usageUser = 0.0f;
        data.usageIdle = 0.0f;
        data.usageNice = 0.0f;
        data.usageIOWait = 0.0f;
        data.usageIRQ = 0.0f;
        data.usageSoftIRQ = 0.0f;
        data.usageGuest = 0.0f;
        data.usageGuestNice = 0.0f;
        data.usageSteal = 0.0f;
        data.timestamp = 0;
    }
    return (data);
}

bool SourceData::SetCurrentCPUData(float loadPercent, float usageSystem, float usageUser, float usageIdle, float usageNice, float usageIOWait, float usageIRQ, float usageSoftIRQ, float usageGuest, float usageGuestNice, float usageSteal, uint64_t timestamp)
{
    if (this->cpuLoadQueue != NULL)
    {
        SourceDataQueueCPUValues data = this->GetCurrentCPUData();
        if (loadPercent != data.loadPercent || usageSystem != data.usageSystem || usageUser != data.usageUser || usageIdle != data.usageIdle || usageNice != data.usageNice || usageIOWait != data.usageIOWait || usageIRQ != data.usageIRQ || usageSoftIRQ != data.usageSoftIRQ || usageGuest != data.usageGuest || usageGuestNice != data.usageGuestNice ||
            usageSteal != data.usageSteal)
        {

            if (!truncateOverflows)
            {
                if (
                    loadPercent < MIN_CPU_USAGE_LIMIT_VALUE || loadPercent > MAX_CPU_USAGE_LIMIT_VALUE ||
                    usageSystem < MIN_CPU_USAGE_LIMIT_VALUE || usageSystem > MAX_CPU_USAGE_LIMIT_VALUE ||
                    usageUser < MIN_CPU_USAGE_LIMIT_VALUE || usageUser > MAX_CPU_USAGE_LIMIT_VALUE ||
                    usageIdle < MIN_CPU_USAGE_LIMIT_VALUE || usageIdle > MAX_CPU_USAGE_LIMIT_VALUE ||
                    usageNice < MIN_CPU_USAGE_LIMIT_VALUE || usageNice > MAX_CPU_USAGE_LIMIT_VALUE ||
                    usageIOWait < MIN_CPU_USAGE_LIMIT_VALUE || usageIOWait > MAX_CPU_USAGE_LIMIT_VALUE ||
                    usageIRQ < MIN_CPU_USAGE_LIMIT_VALUE || usageIRQ > MAX_CPU_USAGE_LIMIT_VALUE ||
                    usageSoftIRQ < MIN_CPU_USAGE_LIMIT_VALUE || usageSoftIRQ > MAX_CPU_USAGE_LIMIT_VALUE ||
                    usageGuest < MIN_CPU_USAGE_LIMIT_VALUE || usageGuest > MAX_CPU_USAGE_LIMIT_VALUE ||
                    usageGuestNice < MIN_CPU_USAGE_LIMIT_VALUE || usageGuestNice > MAX_CPU_USAGE_LIMIT_VALUE ||
                    usageSteal < MIN_CPU_USAGE_LIMIT_VALUE || usageSteal > MAX_CPU_USAGE_LIMIT_VALUE)
                {
                    return (false);
                }
            }
            else
            {
                data.loadPercent = loadPercent < MIN_CPU_USAGE_LIMIT_VALUE ? MIN_CPU_USAGE_LIMIT_VALUE : loadPercent > MAX_CPU_USAGE_LIMIT_VALUE ? MAX_CPU_USAGE_LIMIT_VALUE
                                                                                                                                                 : loadPercent;
                data.usageSystem = usageSystem < MIN_CPU_USAGE_LIMIT_VALUE ? MIN_CPU_USAGE_LIMIT_VALUE : usageSystem > MAX_CPU_USAGE_LIMIT_VALUE ? MAX_CPU_USAGE_LIMIT_VALUE
                                                                                                                                                 : usageSystem;
                data.usageUser = usageUser < MIN_CPU_USAGE_LIMIT_VALUE ? MIN_CPU_USAGE_LIMIT_VALUE : usageUser > MAX_CPU_USAGE_LIMIT_VALUE ? MAX_CPU_USAGE_LIMIT_VALUE
                                                                                                                                           : usageUser;
                data.usageIdle = usageIdle < MIN_CPU_USAGE_LIMIT_VALUE ? MIN_CPU_USAGE_LIMIT_VALUE : usageIdle > MAX_CPU_USAGE_LIMIT_VALUE ? MAX_CPU_USAGE_LIMIT_VALUE
                                                                                                                                           : usageIdle;
                data.usageNice = usageNice < MIN_CPU_USAGE_LIMIT_VALUE ? MIN_CPU_USAGE_LIMIT_VALUE : usageNice > MAX_CPU_USAGE_LIMIT_VALUE ? MAX_CPU_USAGE_LIMIT_VALUE
                                                                                                                                           : usageNice;
                data.usageIOWait = usageIOWait < MIN_CPU_USAGE_LIMIT_VALUE ? MIN_CPU_USAGE_LIMIT_VALUE : usageIOWait > MAX_CPU_USAGE_LIMIT_VALUE ? MAX_CPU_USAGE_LIMIT_VALUE
                                                                                                                                                 : usageIOWait;
                data.usageIRQ = usageIRQ < MIN_CPU_USAGE_LIMIT_VALUE ? MIN_CPU_USAGE_LIMIT_VALUE : usageIRQ > MAX_CPU_USAGE_LIMIT_VALUE ? MAX_CPU_USAGE_LIMIT_VALUE
                                                                                                                                        : usageIRQ;
                data.usageSoftIRQ = usageSoftIRQ < MIN_CPU_USAGE_LIMIT_VALUE ? MIN_CPU_USAGE_LIMIT_VALUE : usageSoftIRQ > MAX_CPU_USAGE_LIMIT_VALUE ? MAX_CPU_USAGE_LIMIT_VALUE
                                                                                                                                                    : usageSoftIRQ;
                data.usageGuest = usageGuest < MIN_CPU_USAGE_LIMIT_VALUE ? MIN_CPU_USAGE_LIMIT_VALUE : usageGuest > MAX_CPU_USAGE_LIMIT_VALUE ? MAX_CPU_USAGE_LIMIT_VALUE
                                                                                                                                              : usageGuest;
                data.usageGuestNice = usageGuestNice < MIN_CPU_USAGE_LIMIT_VALUE ? MIN_CPU_USAGE_LIMIT_VALUE : usageGuestNice > MAX_CPU_USAGE_LIMIT_VALUE ? MAX_CPU_USAGE_LIMIT_VALUE
                                                                                                                                                          : usageGuestNice;
                data.usageSteal = usageSteal < MIN_CPU_USAGE_LIMIT_VALUE ? MIN_CPU_USAGE_LIMIT_VALUE : usageSteal > MAX_CPU_USAGE_LIMIT_VALUE ? MAX_CPU_USAGE_LIMIT_VALUE
                                                                                                                                              : usageSteal;
                data.timestamp = timestamp;
                return (xQueueOverwrite(this->cpuLoadQueue, &data) == pdPASS);
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

SourceDataQueueUsedMemoryValues SourceData::GetCurrentMemoryData(void)
{
    SourceDataQueueUsedMemoryValues data;
    if (this->usedMemoryQueue != nullptr)
    {
        if (xQueuePeek(this->usedMemoryQueue, &data, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS)
        {
        }
    }
    return (data);
}

bool SourceData::SetCurrentMemoryData(
    SourceDataQueueUsedMemoryValues currentData)
{
    if (this->usedMemoryQueue != nullptr)
    {
        SourceDataQueueUsedMemoryValues data = this->GetCurrentMemoryData();
        data = currentData;
        return (xQueueOverwrite(this->usedMemoryQueue, &data) == pdPASS);
    }
    else
    {
        return (false);
    }
}

// CPU TEMPERATURE

SourceDataQueueCPUTemperatureValue SourceData::GetCurrentCPUTemperature(void)
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

bool SourceData::SetCurrentCPUTemperature(float celsious, uint64_t timestamp)
{
    if (this->cpuTemperatureQueue != NULL)
    {
        SourceDataQueueCPUTemperatureValue data = this->GetCurrentCPUTemperature();
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

SourceDataQueueUptimeValue SourceData::GetCurrentUptime(void)
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

bool SourceData::SetCurrentUptime(uint64_t seconds, uint64_t timestamp)
{
    if (this->systemUptimeQueue != NULL)
    {
        SourceDataQueueUptimeValue data = this->GetCurrentUptime();
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

SourceDataQueueNetworkingLimitsValue SourceData::GetNetworkLimits(void)
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

bool SourceData::SetNetworkLimits(uint64_t byteDownloadLimit, uint64_t byteUploadLimit)
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

SourceDataQueueNetworkingValue SourceData::GetCurrentNetworkDownload(void)
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

bool SourceData::SetCurrentNetworkDownload(uint64_t totalBytes, uint64_t timestamp)
{
    if (this->networkingDownloadQueue)
    {
        SourceDataQueueNetworkingValue data = this->GetCurrentNetworkDownload();
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

SourceDataQueueNetworkingValue SourceData::GetCurrentNetworkUpload(void)
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

bool SourceData::SetCurrentNetworkUpload(uint64_t totalBytes, uint64_t timestamp)
{
    if (this->networkingUploadQueue)
    {
        SourceDataQueueNetworkingValue data = this->GetCurrentNetworkUpload();
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
