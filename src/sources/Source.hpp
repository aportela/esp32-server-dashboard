#ifndef ESP32_SERVER_DASHBOARD_SOURCE_H
#define ESP32_SERVER_DASHBOARD_SOURCE_H

#include <stdint.h>
#include "ISource.hpp"
#include "SourceData.hpp"

#define MIN_CPU_LOAD 0
#define MAX_CPU_LOAD 100

typedef void (*onFloatValueChangedCallback)(float, uint64_t);
typedef void (*onUint64tValueChangedCallback)(uint64_t, uint64_t);

class Source : public ISource
{
protected:
    SourceData *sourceData = nullptr;
    onFloatValueChangedCallback onCPULoadChanged = nullptr;
    onUint64tValueChangedCallback onTotalMemoryChanged = nullptr;
    onUint64tValueChangedCallback onUsedMemoryChanged = nullptr;
    onFloatValueChangedCallback onCPUTemperatureChangedCallback = nullptr;

public:
    Source(SourceData *sourceData);
    virtual ~Source();
};

#endif // ESP32_SERVER_DASHBOARD_SOURCE_H
