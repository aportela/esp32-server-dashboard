#ifndef ESP32_SERVER_DASHBOARD_SOURCE_H
#define ESP32_SERVER_DASHBOARD_SOURCE_H

#include <stdint.h>
#include "ISource.hpp"
#include "SourceData.hpp"

#define MIN_CPU_LOAD 0
#define MAX_CPU_LOAD 100

class Source : public ISource
{
protected:
    SourceData *sourceData = nullptr;
    static void (*onCPULoadChangedCallback)(float value, uint64_t timestamp);
    static void (*onTotalMemoryChangedCallback)(uint64_t value, uint64_t timestamp);
    static void (*onUsedMemoryChangedCallback)(uint64_t value, uint64_t timestamp);
    static void (*onCPUTemperatureChangedCallback)(float value, uint64_t timestamp);

public:
    Source(SourceData *sourceData);
    virtual ~Source();

    void onCPULoadChanged(void (*callback)(float value, uint64_t timestamp));
    void onTotalMemoryChanged(void (*callback)(uint64_t value, uint64_t timestamp));
    void onUsedMemoryChanged(void (*callback)(uint64_t value, uint64_t timestamp));
    void onCPUTemperatureChanged(void (*callback)(float value, uint64_t timestamp));
};

#endif
