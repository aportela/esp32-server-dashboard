
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
typedef struct SourceDataQueueCPUValues
{
    float loadPercent; // value is equal to => 100 - usageIdle
    float usageSystem;
    float usageUser;
    float usageIdle;
    float usageNice;
    float usageIOWait;
    float usageIRQ;
    float usageSoftIRQ;
    float usageGuest;
    float usageGuestNice;
    float usageSteal;

    uint64_t timestamp;
};

// https://github.com/influxdata/telegraf/blob/master/plugins/inputs/mem/README.md
typedef struct SourceDataQueueUsedMemoryValue
{
    uint64_t active;
    uint64_t available;
    float availablePercent;
    uint64_t buffered;
    uint64_t cached;
    uint64_t commitLimit;
    uint64_t committedAs;
    uint64_t dirty;
    uint64_t free;
    uint64_t highFree;
    uint64_t highTotal;
    uint64_t hugePagesFree;
    uint64_t hugePageSize;
    uint64_t hugePagesTotal;
    uint64_t inactive;
    uint64_t laundry;
    uint64_t lowFree;
    uint64_t lowTotal;
    uint64_t mapped;
    uint64_t pageTables;
    uint64_t shared;
    uint64_t slab;
    uint64_t sreclaimable;
    uint64_t sunreclaim;
    uint64_t swapCached;
    uint64_t swapFree;
    uint64_t swapTotal;
    uint64_t total;
    uint64_t used;
    float usedPercent;
    uint64_t vmallocChunk;
    uint64_t vmallocTotal;
    uint64_t vmallocUsed;
    uint64_t wired;
    uint64_t writeBack;
    uint64_t writeBackTmp;

    uint64_t timestamp;
};

typedef struct SourceDataQueueCPUTemperatureValue
{
    float celsious;
    uint64_t timestamp;
};

typedef struct SourceDataQueueUptimeValue
{
    uint64_t seconds;
    uint64_t timestamp;
};

typedef struct SourceDataQueueNetworkingValue
{
    uint64_t totalBytesTransfered;
    uint64_t currentBandwidthBytesPerSecond;
    uint64_t timestamp;
};

typedef struct SourceDataQueueNetworkingLimitsValue
{
    uint64_t byteDownloadLimit;
    uint64_t byteUploadLimit;
};

#define HOSTNAME_ARR_LENGTH 17

class SourceData
{
private:
    char hostname[HOSTNAME_ARR_LENGTH] = {'\0'};
    // TODO: REMOVE, ALWAYS SAVE ORIGINAL DATA => TRUNCATE ON EXTERNAL CALLS
    bool truncateOverflows = false;
    bool fixedNetworkingLimits = false;

    QueueHandle_t cpuLoadQueue;
    QueueHandle_t usedMemoryQueue;
    QueueHandle_t cpuTemperatureQueue;
    QueueHandle_t systemUptimeQueue;
    QueueHandle_t networkingDownloadQueue;
    QueueHandle_t networkingUploadQueue;
    QueueHandle_t networkingLimitsQueue;

public:
    static const uint8_t MAX_HOSTNAME_LENGTH = HOSTNAME_ARR_LENGTH - 1;

    SourceData(bool truncateOverflows, uint64_t totalNetworkDownloadBandwidthLimit = 0, uint64_t totalNetworkUploadBandwidthLimit = 0);
    ~SourceData();

    bool HasFixedNetworkingLimits(void);

    bool SetHostname(const char *hostname);
    void GetHostname(char *hostname, size_t count);

    // CPU LOAD

    SourceDataQueueCPUValues GetCurrentCPUData(void);
    bool SetCurrentCPUData(float loadPercent, float usageSystem, float usageUser, float usageIdle, float usageNice, float usageIOWait, float usageIRQ, float usageSoftIRQ, float usageGuest, float usageGuestNice, float usageSteal, uint64_t timestamp);

    // MEMORY

    SourceDataQueueUsedMemoryValue GetCurrentUsedMemory(void);
    bool SetCurrentUsedMemory(uint64_t usedBytes, uint64_t totalBytes, uint64_t timestamp);
    bool SetCurrentMemoryData(
        uint64_t active,
        uint64_t available,
        float availablePercent,
        uint64_t buffered,
        uint64_t cached,
        uint64_t commitLimit,
        uint64_t committedAs,
        uint64_t dirty,
        uint64_t free,
        uint64_t highFree,
        uint64_t highTotal,
        uint64_t hugePagesFree,
        uint64_t hugePageSize,
        uint64_t hugePagesTotal,
        uint64_t inactive,
        uint64_t laundry,
        uint64_t lowFree,
        uint64_t lowTotal,
        uint64_t mapped,
        uint64_t pageTables,
        uint64_t shared,
        uint64_t slab,
        uint64_t sreclaimable,
        uint64_t sunreclaim,
        uint64_t swapCached,
        uint64_t swapFree,
        uint64_t swapTotal,
        uint64_t total,
        uint64_t used,
        float usedPercent,
        uint64_t vmallocChunk,
        uint64_t vmallocTotal,
        uint64_t vmallocUsed,
        uint64_t wired,
        uint64_t writeBack,
        uint64_t writeBackTmp,
        uint64_t timestamp);

    // CPU TEMPERATURE

    SourceDataQueueCPUTemperatureValue GetCurrentCPUTemperature(void);
    bool SetCurrentCPUTemperature(float celsious, uint64_t timestamp);

    // SYSTEM

    SourceDataQueueUptimeValue GetCurrentUptime(void);
    bool SetCurrentUptime(uint64_t seconds, uint64_t timestamp);

    // NET COMMON

    SourceDataQueueNetworkingLimitsValue GetNetworkLimits(void);
    bool SetNetworkLimits(uint64_t byteDownloadLimit, uint64_t byteUploadLimit);
    SourceDataQueueNetworkingValue GetCurrentNetworkDownload(void);
    bool SetCurrentNetworkDownload(uint64_t totalBytes, uint64_t timestamp);
    SourceDataQueueNetworkingValue GetCurrentNetworkUpload(void);
    bool SetCurrentNetworkUpload(uint64_t totalBytes, uint64_t timestamp);
};

#endif // ESP32_SERVER_DASHBOARD_SOURCE_DATA_H
