#ifndef ESP32_SERVER_DASHBOARD_SOURCE_INTERFACE_H
#define ESP32_SERVER_DASHBOARD_SOURCE_INTERFACE_H

#include <stdint.h>
#include "../EntityType.hpp"

class ISource
{
public:
    virtual ~ISource() {}
    virtual bool changed(EntityType entityType, uint64_t fromTimestamp) = 0;
    virtual void setMin(EntityType entityType, uint64_t min) = 0;
    virtual void setMax(EntityType entityType, uint64_t max) = 0;
    virtual uint64_t getMin(EntityType entityType) = 0;
    virtual uint64_t getMax(EntityType entityType) = 0;
    virtual uint64_t getPreviousValue(EntityType entityType) = 0;
    virtual uint64_t getCurrentValue(EntityType entityType) = 0;
};

#endif
