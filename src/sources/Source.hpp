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
    void (*onCPULoadChangedCallback)(float, uint64_t) = nullptr;
    void (*onTotalMemoryChangedCallback)(uint64_t, uint64_t) = nullptr;
    void (*onUsedMemoryChangedCallback)(uint64_t, uint64_t) = nullptr;
    void (*onCPUTemperatureChangedCallback)(float, uint64_t) = nullptr;

public:
    Source(SourceData *sourceData);
    virtual ~Source();

    void onCPULoadChanged(void (*callback)(float, uint64_t));
    void onTotalMemoryChanged(void (*callback)(uint64_t, uint64_t));
    void onUsedMemoryChanged(void (*callback)(uint64_t, uint64_t));
    void onCPUTemperatureChanged(void (*callback)(float, uint64_t));
};

#endif
