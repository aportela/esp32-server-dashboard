#include "Source.hpp"

Source::Source(SourceData *sourceData)
{
    if (sourceData != nullptr)
    {
        this->sourceData = sourceData;
    }
}

Source::~Source()
{
    if (this->sourceData != nullptr)
    {
        this->sourceData = nullptr;
    }
    Source::onCPULoadChangedCallback = nullptr;
}

void Source::onCPULoadChanged(void (*callback)(float, uint64_t))
{
    this->onCPULoadChangedCallback = callback;
}

void Source::onTotalMemoryChanged(void (*callback)(uint64_t, uint64_t))
{
    this->onTotalMemoryChangedCallback = callback;
}

void Source::onUsedMemoryChanged(void (*callback)(uint64_t, uint64_t))
{
    this->onUsedMemoryChangedCallback = callback;
}

void Source::onCPUTemperatureChanged(void (*callback)(float, uint64_t))
{
    this->onCPUTemperatureChangedCallback = callback;
}
