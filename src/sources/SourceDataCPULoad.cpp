#include "SourceDataCPULoad.hpp"

SourceDataCPULoad::SourceDataCPULoad(uint8_t min, uint8_t max) : min(min), max(max)
{
    this->current = 0;
}

SourceDataCPULoad::~SourceDataCPULoad()
{
}

uint8_t SourceDataCPULoad::getMin(void)
{
    return (this->min);
}

uint8_t SourceDataCPULoad::getMax(void)
{
    return (this->max);
}

void SourceDataCPULoad::setCurrent(uint8_t value)
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

uint8_t SourceDataCPULoad::getCurrent(void)
{
    return (this->current);
}