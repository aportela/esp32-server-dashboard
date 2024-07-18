#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_CPU_TEMPERATURE
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_CPU_TEMPERATURE

#include "SourceData.hpp"

class SourceDataCPUTemperature : public SourceData
{

public:
    SourceDataCPUTemperature(uint8_t min, uint8_t max);
    ~SourceDataCPUTemperature();
};

#endif
