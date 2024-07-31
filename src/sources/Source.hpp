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

public:
    Source(SourceData *sourceData);
    virtual ~Source();
};

#endif
