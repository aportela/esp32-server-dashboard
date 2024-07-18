#include "SourceData.hpp"

SourceData::SourceData(uint8_t min, uint8_t max) : min(min), max(max)
{
    this->current = 0;
}

SourceData::~SourceData()
{
}

uint8_t SourceData::getMin(void)
{
    return (this->min);
}

uint8_t SourceData::getMax(void)
{
    return (this->max);
}

void SourceData::setCurrent(uint8_t value)
{
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

uint8_t SourceData::getCurrent(void)
{
    return (this->current);
}