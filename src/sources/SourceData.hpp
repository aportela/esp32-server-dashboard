#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_H
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_H

#include <cstdint>
#include <cstddef>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#define QUEUE_PEEK_MS_TO_TICKS_TIMEOUT 5

#define MAX_NETWORK_INTERFACE_ID_LENGTH 65

#define MIN_CPU_LOAD 0
#define MAX_CPU_LOAD 100

#define MIN_CPU_TEMPERATURE 0.0f
#define MAX_CPU_TEMPERATURE 100.0f

// TODO: https://github.com/influxdata/telegraf/blob/master/plugins/inputs/cpu/README.md
struct SourceDataQueueCPUValues
{
    float timeUser = 0.0f;
    float timeSystem = 0.0f;
    float timeIdle = 0.0f;
    float timeActive = 0.0f;
    float timeNice = 0.0f;
    float timeIOWait = 0.0f;
    float timeIRQ = 0.0f;
    float timeSoftIRQ = 0.0f;
    float timeSteal = 0.0f;
    float timeGuest = 0.0f;
    float timeGuestNice = 0.0f;
    float usageUser = 0.0f;
    float usageSystem = 0.0f;
    float usageIdle = 0.0f;
    float usageActive = 0.0f;
    float usageNice = 0.0f;
    float usageIOWait = 0.0f;
    float usageIRQ = 0.0f;
    float usageSoftIRQ = 0.0f;
    float usageSteal = 0.0f;
    float usageGuest = 0.0f;
    float usageGuestNice = 0.0f;

    uint64_t timestamp = 0;

    SourceDataQueueCPUValues() = default;
};

// https://github.com/influxdata/telegraf/blob/master/plugins/inputs/mem/README.md

struct SourceDataQueueUsedMemoryValues
{
    uint64_t active = 0;
    uint64_t available = 0;
    float availablePercent = 0.0f;
    uint64_t buffered = 0;
    uint64_t cached = 0;
    uint64_t commitLimit = 0;
    uint64_t committedAs = 0;
    uint64_t dirty = 0;
    uint64_t free = 0;
    uint64_t highFree = 0;
    uint64_t highTotal = 0;
    uint64_t hugePagesFree = 0;
    uint64_t hugePageSize = 0;
    uint64_t hugePagesTotal = 0;
    uint64_t inactive = 0;
    uint64_t laundry = 0;
    uint64_t lowFree = 0;
    uint64_t lowTotal = 0;
    uint64_t mapped = 0;
    uint64_t pageTables = 0;
    uint64_t shared = 0;
    uint64_t slab = 0;
    uint64_t sreclaimable = 0;
    uint64_t sunreclaim = 0;
    uint64_t swapCached = 0;
    uint64_t swapFree = 0;
    uint64_t swapTotal = 0;
    uint64_t total = 0;
    uint64_t used = 0;
    float usedPercent = 0.0f;
    uint64_t vmallocChunk = 0;
    uint64_t vmallocTotal = 0;
    uint64_t vmallocUsed = 0;
    uint64_t wired = 0;
    uint64_t writeBack = 0;
    uint64_t writeBackTmp = 0;

    uint64_t timestamp = 0;

    SourceDataQueueUsedMemoryValues() = default;
};

struct SourceDataQueueCPUTemperatureValue
{
    float celsious = 0.0f;

    uint64_t timestamp = 0;

    SourceDataQueueCPUTemperatureValue() = default;
};

struct SourceDataQueueUptimeValue
{
    uint64_t seconds = 0;

    uint64_t timestamp = 0;

    SourceDataQueueUptimeValue() = default;
};

struct SourceDataQueueNetworkingValue
{
    uint64_t bytesSent = 0;
    uint64_t bytesRecv = 0;
    uint64_t packetsSent = 0;
    uint64_t packetsRecv = 0;
    uint64_t errIn = 0;
    uint64_t errOut = 0;
    uint64_t dropIn = 0;
    uint64_t dropOut = 0;
    uint64_t speed = 0;

    uint64_t timestamp = 0;

    SourceDataQueueNetworkingValue() = default;
};

struct SourceDataQueueNetworkingLimitsValue
{
    uint64_t byteDownloadLimit = 0;
    uint64_t byteUploadLimit = 0;

    SourceDataQueueNetworkingLimitsValue() = default;
    SourceDataQueueNetworkingLimitsValue(uint64_t downloadLimit, uint64_t uploadLimit)
        : byteDownloadLimit(downloadLimit), byteUploadLimit(uploadLimit) {}
};

#define HOSTNAME_ARR_LENGTH 17

class SourceData
{
private:
    char hostname[HOSTNAME_ARR_LENGTH] = {'\0'};
    // TODO: REMOVE, ALWAYS SAVE ORIGINAL DATA => TRUNCATE ON EXTERNAL CALLS
    bool truncateOverflows = false;
    bool fixedNetworkingLimits = false;

    QueueHandle_t cpuQueue;
    QueueHandle_t memoryQueue;
    QueueHandle_t cpuTemperatureQueue;
    QueueHandle_t systemUptimeQueue;
    QueueHandle_t networkingQueue;
    QueueHandle_t networkingLimitsQueue;

public:
    static const uint8_t MAX_HOSTNAME_LENGTH = HOSTNAME_ARR_LENGTH - 1;

    SourceData(bool truncateOverflows, uint64_t totalNetworkDownloadBandwidthLimit = 0, uint64_t totalNetworkUploadBandwidthLimit = 0);
    ~SourceData();

    bool HasFixedNetworkingLimits(void);

    bool SetHostname(const char *hostname);
    void GetHostname(char *hostname, size_t count);

    // CPU LOAD

    bool GetCurrentCPUData(SourceDataQueueCPUValues &currentData);
    bool SetCurrentCPUData(SourceDataQueueCPUValues currentData);

    // MEMORY

    bool GetCurrentMemoryData(SourceDataQueueUsedMemoryValues &currentData);
    bool SetCurrentMemoryData(SourceDataQueueUsedMemoryValues currentData);

    // CPU TEMPERATURE

    bool GetCurrentCPUTemperature(SourceDataQueueCPUTemperatureValue &currentData);
    bool SetCurrentCPUTemperature(SourceDataQueueCPUTemperatureValue currentData);

    // SYSTEM

    bool GetCurrentUptime(SourceDataQueueUptimeValue &currentData);
    bool SetCurrentUptime(SourceDataQueueUptimeValue currentData);

    // NET COMMON

    bool GetNetworkLimits(SourceDataQueueNetworkingLimitsValue &currentData);
    bool SetNetworkLimits(SourceDataQueueNetworkingLimitsValue currentData);
    bool GetCurrentNetwork(SourceDataQueueNetworkingValue &currentData);
    bool SetCurrentNetwork(SourceDataQueueNetworkingValue currentData);
};

#endif // ESP32_SERVER_DASHBOARD_SOURCE_DATA_H
