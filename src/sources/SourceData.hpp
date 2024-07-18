#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA

#include <stdint.h>

enum SourceDataType
{
    SDT_CPU,
    SDT_MEMORY,
    SDT_NETWORK,
    SDT_TEMPERATURE
};

class SourceData
{
private:
    uint8_t min;
    uint8_t max;
    uint8_t current;

public:
    SourceData(uint8_t min, uint8_t max);
    ~SourceData();
    uint8_t getMin(void);
    uint8_t getMax(void);
    void setCurrent(uint8_t value);
    uint8_t getCurrent(void);
};

#endif
