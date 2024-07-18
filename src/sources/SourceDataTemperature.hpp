#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_TEMPERATURE
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_TEMPERATURE

#include "SourceData.hpp"

class SourceDataTemperature : public SourceData
{
public:
    uint8_t globalTemperature;
};

#endif
