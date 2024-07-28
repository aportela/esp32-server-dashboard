#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_H
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_H

#include <stdint.h>

class EntityData
{
private:
    uint64_t min;
    uint64_t max;
    uint64_t previous;
    uint64_t current;
    uint64_t currentTimestamp;

public:
    EntityData(uint64_t min, uint64_t max);
    ~EntityData();
    uint64_t getMin(void);
    uint64_t getMax(void);
    void setCurrent(uint64_t value, uint64_t timestamp);
    uint64_t getPrevious(void);
    uint64_t getCurrent(void);
    uint64_t getCurrentTimestamp(void);
    bool changed(uint64_t fromTimestamp);
};

#endif
