
#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_H
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_H

#include <cstdint>
#include <cstddef>
#include "../EntityType.hpp"

// TODO: allow data with decimals
class SourceData
{
private:
    // CPU LOAD
    uint64_t minCPULoad = 0;
    uint64_t maxCPULoad = 0;
    float previousCPULoad = 0.0f;
    float currentCPULoad = 0.0f;
    uint64_t currentCPULoadTimestamp = 0;
    // MEMORY
    // CPU TEMPERATURE
    // DOWNLOAD BANDWIDTH
    // UPLOAD BANDWIDTH

public:
    SourceData(uint8_t min, uint8_t max);
    ~SourceData();
    // CPU LOAD
    uint8_t getMinCPULoad(void);
    uint8_t getMaxCPULoad(void);
    void setMinCPULoad(uint8_t value);
    void setMaxCPULoad(uint8_t value);
    float getCurrentCPULoad(void);
    void getCurrentCPULoad(char *buffer, size_t buffer_size, uint8_t decimals = 1);
    float getPreviousCPULoad(void);
    uint64_t getCurrentCPULoadTimestamp(void);
    bool changedCPULoad(uint64_t fromTimestamp);
    bool setCurrentCPULoad(float value, uint64_t timestamp);
    // MEMORY
    // CPU TEMPERATURE
    // DOWNLOAD BANDWIDTH
    // UPLOAD BANDWIDTH

    void refresh(void);
};

#endif
