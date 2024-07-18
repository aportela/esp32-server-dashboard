#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_CPU_LOAD
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_CPU_LOAD

#include "SourceData.hpp"
#include <stdint.h>

class SourceDataCPULoad : public SourceData
{
private:
    uint8_t min;
    uint8_t max;
    uint8_t current;

public:
    SourceDataCPULoad(uint8_t min, uint8_t max);
    ~SourceDataCPULoad();
    uint8_t getMin(void);
    uint8_t getMax(void);
    void setCurrent(uint8_t value);
    uint8_t getCurrent(void);
};

#endif
