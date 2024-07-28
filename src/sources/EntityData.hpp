#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_H
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_H

#include <stdint.h>

// TODO: allow data with decimals
class EntityData
{
private:
    uint64_t min;
    uint64_t max;
    uint64_t previousValue;
    uint64_t currentValue;
    uint64_t currentTimestamp;

public:
    EntityData(void);
    EntityData(uint64_t min, uint64_t max);
    ~EntityData();
    void setMin(uint64_t min);
    void setMax(uint64_t max);
    uint64_t getMin(void);
    uint64_t getMax(void);
    void setCurrentValue(uint64_t value, uint64_t timestamp);
    uint64_t getPreviousValue(void);
    uint64_t getCurrentValue(void);
    uint64_t getCurrentTimestamp(void);
    bool changed(uint64_t fromTimestamp);
};

#endif
