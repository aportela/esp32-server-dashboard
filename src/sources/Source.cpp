#include "Source.hpp"

void (*Source::onCPULoadChangedCallback)(float value, uint64_t timestamp) = nullptr;
void (*Source::onTotalMemoryChangedCallback)(uint64_t value, uint64_t timestamp) = nullptr;
void (*Source::onUsedMemoryChangedCallback)(uint64_t value, uint64_t timestamp) = nullptr;
void (*Source::onCPUTemperatureChangedCallback)(float value, uint64_t timestamp) = nullptr;

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

void Source::onCPULoadChanged(void (*callback)(float value, uint64_t timestamp))
{
    Source::onCPULoadChangedCallback = callback;
}

void Source::onTotalMemoryChanged(void (*callback)(uint64_t value, uint64_t timestamp))
{
    Source::onTotalMemoryChangedCallback = callback;
}

void Source::onUsedMemoryChanged(void (*callback)(uint64_t value, uint64_t timestamp))
{
    Source::onUsedMemoryChangedCallback = callback;
}

void Source::onCPUTemperatureChanged(void (*callback)(float value, uint64_t timestamp))
{
    Source::onCPUTemperatureChangedCallback = callback;
}
