#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_MEMORY
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_MEMORY

#include "SourceData.hpp"

class SourceDataMemory : public SourceData
{
public:
    uint64_t totalMemory;
    uint64_t usedMemory;
};

#endif
