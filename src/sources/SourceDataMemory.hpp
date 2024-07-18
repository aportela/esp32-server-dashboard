#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_MEMORY
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_MEMORY

#include "SourceData.hpp"

class SourceDataMemory : public SourceData
{

public:
    SourceDataMemory(uint8_t min, uint8_t max);
    ~SourceDataMemory();
};

#endif
