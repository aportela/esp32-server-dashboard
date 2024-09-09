
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
    this->networkingQueue = xQueueCreate(1, sizeof(SourceDataQueueNetworkingValue));
    this->networkingLimitsQueue = xQueueCreate(1, sizeof(SourceDataQueueNetworkingLimitsValue));
    this->fixedNetworkingLimits = totalNetworkDownloadBandwidthLimit > 0 && totalNetworkUploadBandwidthLimit > 0;
    SourceDataQueueNetworkingLimitsValue networkingLimits = {totalNetworkDownloadBandwidthLimit, totalNetworkUploadBandwidthLimit};
    this->SetNetworkLimits(networkingLimits);
}

SourceData::~SourceData()
{
    vQueueDelete(this->cpuLoadQueue);
    vQueueDelete(this->usedMemoryQueue);
    vQueueDelete(this->cpuTemperatureQueue);
    vQueueDelete(this->systemUptimeQueue);
    vQueueDelete(this->networkingQueue);
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

bool SourceData::GetCurrentCPUData(SourceDataQueueCPUValues &currentData)
{
    return (this->cpuLoadQueue != nullptr && xQueuePeek(this->cpuLoadQueue, &currentData, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS);
}

bool SourceData::SetCurrentCPUData(SourceDataQueueCPUValues currentData)
{
    return (this->cpuLoadQueue != nullptr && xQueueOverwrite(this->cpuLoadQueue, &currentData) == pdPASS);
}

// MEMORY

bool SourceData::GetCurrentMemoryData(SourceDataQueueUsedMemoryValues &currentData)
{
    return (this->usedMemoryQueue != nullptr && xQueuePeek(this->usedMemoryQueue, &currentData, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS);
}

bool SourceData::SetCurrentMemoryData(SourceDataQueueUsedMemoryValues currentData)
{
    return (this->usedMemoryQueue != nullptr && xQueueOverwrite(this->usedMemoryQueue, &currentData) == pdPASS);
}

// CPU TEMPERATURE

bool SourceData::GetCurrentCPUTemperature(SourceDataQueueCPUTemperatureValue &currentData)
{
    return (this->cpuTemperatureQueue != nullptr && xQueuePeek(this->cpuTemperatureQueue, &currentData, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS);
}

bool SourceData::SetCurrentCPUTemperature(SourceDataQueueCPUTemperatureValue currentData)
{
    return (this->cpuTemperatureQueue != nullptr && xQueueOverwrite(this->cpuTemperatureQueue, &currentData) == pdPASS);
}

// SYSTEM

bool SourceData::GetCurrentUptime(SourceDataQueueUptimeValue &currentData)
{
    return (this->systemUptimeQueue != nullptr && xQueuePeek(this->systemUptimeQueue, &currentData, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS);
}

bool SourceData::SetCurrentUptime(SourceDataQueueUptimeValue currentData)
{
    return (this->systemUptimeQueue != nullptr && xQueueOverwrite(this->systemUptimeQueue, &currentData) == pdPASS);
}

// NET COMMON

bool SourceData::GetNetworkLimits(SourceDataQueueNetworkingLimitsValue &currentData)
{
    return (this->networkingLimitsQueue != nullptr && xQueuePeek(this->networkingLimitsQueue, &currentData, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS);
}

bool SourceData::SetNetworkLimits(SourceDataQueueNetworkingLimitsValue currentData)
{
    return (this->networkingLimitsQueue != nullptr && xQueueOverwrite(this->networkingLimitsQueue, &currentData) == pdPASS);
}

bool SourceData::GetCurrentNetwork(SourceDataQueueNetworkingValue &currentData)
{
    return (this->networkingQueue != nullptr && xQueuePeek(this->networkingQueue, &currentData, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) != pdPASS);
}

bool SourceData::SetCurrentNetwork(SourceDataQueueNetworkingValue currentData)
{
    return (this->networkingQueue != nullptr && xQueueOverwrite(this->networkingQueue, &currentData) == pdPASS);
}
