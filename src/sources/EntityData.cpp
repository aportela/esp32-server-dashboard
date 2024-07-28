#include "EntityData.hpp"

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
    this->previous = 1;
    this->current = 0;
    this->currentTimestamp = 0;
}

EntityData::~EntityData()
{
}

uint64_t EntityData::getMin(void)
{
    return (this->min);
}

uint64_t EntityData::getMax(void)
{
    return (this->max);
}

void EntityData::setCurrent(uint64_t value, uint64_t timestamp)
{
    if (value != this->current)
    {
        this->previous = this->current;
        if (value >= this->min && value <= this->max)
        {
            this->current = value;
        }
        else if (value < this->min)
        {
            this->current = this->min;
        }
        else if (value > this->max)
        {
            this->current = this->max;
        }
    }
    this->currentTimestamp = timestamp;
}

uint64_t EntityData::getPrevious(void)
{
    return (this->previous);
}

uint64_t EntityData::getCurrent(void)
{
    return (this->current);
}

uint64_t EntityData::getCurrentTimestamp(void)
{
    return (this->currentTimestamp);
}

bool EntityData::changed(uint64_t fromTimestamp)
{
    return (fromTimestamp != this->currentTimestamp);
}
