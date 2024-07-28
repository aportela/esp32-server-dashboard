#include "EntityData.hpp"

EntityData::EntityData(void)
{
    this->min = 0;
    this->max = 0;
    this->previousValue = 1;
    this->currentValue = 0;
    this->currentTimestamp = 0;
}

EntityData::EntityData(uint64_t min, uint64_t max)
{
    // security check to avoid a possible exchange of values ​​that could create a failure
    if (min <= max)
    {
        this->min = min;
        this->max = max;
    }
    else
    {
        // failover
        this->min = max;
        this->max = min;
    }
    this->previousValue = 1;
    this->currentValue = 0;
    this->currentTimestamp = 0;
}

EntityData::~EntityData()
{
}

void EntityData::setMin(uint64_t min)
{
    this->min = min;
}

void EntityData::setMax(uint64_t max)
{
    this->max = max;
}

uint64_t EntityData::getMin(void)
{
    return (this->min);
}

uint64_t EntityData::getMax(void)
{
    return (this->max);
}

void EntityData::setCurrentValue(uint64_t value, uint64_t timestamp)
{
    if (value != this->currentValue)
    {
        this->previousValue = this->currentValue;
        if (value >= this->min && value <= this->max)
        {
            this->currentValue = value;
        }
        else if (value < this->min)
        {
            this->currentValue = this->min;
        }
        else if (value > this->max)
        {
            this->currentValue = this->max;
        }
    }
    else
    {
        this->previousValue = this->currentValue;
    }
    this->currentTimestamp = timestamp;
}

uint64_t EntityData::getPreviousValue(void)
{
    return (this->previousValue);
}

uint64_t EntityData::getCurrentValue(void)
{
    return (this->currentValue);
}

uint64_t EntityData::getCurrentTimestamp(void)
{
    return (this->currentTimestamp);
}

bool EntityData::changed(uint64_t fromTimestamp)
{
    return (fromTimestamp != this->currentTimestamp);
}
