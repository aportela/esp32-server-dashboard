#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA

#include <stdint.h>

class SourceData
{
private:
    uint64_t min;
    uint64_t max;
    uint64_t current;

public:
    SourceData(uint64_t min, uint64_t max);
    ~SourceData();
    uint64_t getMin(void);
    uint64_t getMax(void);
    void setCurrent(uint64_t value);
    uint64_t getCurrent(void);
};

#endif
