#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_CPU_LOAD
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_CPU_LOAD

#include "SourceData.hpp"

class SourceDataCPULoad : public SourceData
{

public:
    SourceDataCPULoad(uint8_t min, uint8_t max);
    ~SourceDataCPULoad();
};

#endif
