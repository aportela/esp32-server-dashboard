#include "Source.hpp"

Source::Source()
{
    this->currentGlobalCPULoadData = new EntityData();
    this->currentUsedMemoryData = new EntityData();
    this->currentGlobalCPUTemperatureData = new EntityData();
    this->currentNetworkDownloadUsedBandwithData = new EntityData();
    this->currentNetworkUploadUsedBandwithData = new EntityData();
}

Source::~Source()
{
    if (this->currentGlobalCPULoadData != nullptr)
    {
        delete this->currentGlobalCPULoadData;
        this->currentGlobalCPULoadData = nullptr;
    }
    if (this->currentUsedMemoryData != nullptr)
    {
        delete this->currentUsedMemoryData;
        this->currentUsedMemoryData = nullptr;
    }
    if (this->currentGlobalCPUTemperatureData != nullptr)
    {
        delete this->currentGlobalCPUTemperatureData;
        this->currentGlobalCPUTemperatureData = nullptr;
    }
    if (this->currentNetworkDownloadUsedBandwithData != nullptr)
    {
        delete this->currentNetworkDownloadUsedBandwithData;
        this->currentNetworkDownloadUsedBandwithData = nullptr;
    }
    if (this->currentNetworkUploadUsedBandwithData != nullptr)
    {
        delete this->currentNetworkUploadUsedBandwithData;
        this->currentNetworkUploadUsedBandwithData = nullptr;
    }
}

bool Source::changed(EntityType entity, uint64_t fromTimestamp) const
{
    switch (entity)
    {
    case ET_GLOBAL_CPU_LOAD:
        return (this->currentGlobalCPULoadData->changed(fromTimestamp));
        break;
    case ET_USED_MEMORY:
        return (this->currentUsedMemoryData->changed(fromTimestamp));
        break;
    case ET_GLOBAL_CPU_TEMPERATURE:
        return (this->currentGlobalCPUTemperatureData->changed(fromTimestamp));
        break;
    case ET_NETWORK_BANDWITH_DOWNLOAD_SPEED:
        return (this->currentNetworkDownloadUsedBandwithData->changed(fromTimestamp));
        break;
    case ET_NETWORK_BANDWITH_UPLOAD_SPEED:
        return (this->currentNetworkUploadUsedBandwithData->changed(fromTimestamp));
        break;
    default:
        return (false);
        break;
    }
}

void Source::setMin(EntityType entity, uint64_t min)
{
    switch (entity)
    {
    case ET_GLOBAL_CPU_LOAD:
        return (this->currentGlobalCPULoadData->setMin(min));
        break;
    case ET_USED_MEMORY:
        return (this->currentUsedMemoryData->setMin(min));
        break;
    case ET_GLOBAL_CPU_TEMPERATURE:
        return (this->currentGlobalCPUTemperatureData->setMin(min));
        break;
    case ET_NETWORK_BANDWITH_DOWNLOAD_SPEED:
        return (this->currentNetworkDownloadUsedBandwithData->setMin(min));
        break;
    case ET_NETWORK_BANDWITH_UPLOAD_SPEED:
        return (this->currentNetworkUploadUsedBandwithData->setMin(min));
        break;
    default:
        break;
    }
}

void Source::setMax(EntityType entity, uint64_t max)
{
    switch (entity)
    {
    case ET_GLOBAL_CPU_LOAD:
        return (this->currentGlobalCPULoadData->setMax(max));
        break;
    case ET_USED_MEMORY:
        return (this->currentUsedMemoryData->setMax(max));
        break;
    case ET_GLOBAL_CPU_TEMPERATURE:
        return (this->currentGlobalCPUTemperatureData->setMax(max));
        break;
    case ET_NETWORK_BANDWITH_DOWNLOAD_SPEED:
        return (this->currentNetworkDownloadUsedBandwithData->setMax(max));
        break;
    case ET_NETWORK_BANDWITH_UPLOAD_SPEED:
        return (this->currentNetworkUploadUsedBandwithData->setMax(max));
        break;
    default:
        break;
    }
}

uint64_t Source::getMin(EntityType entity) const
{
    switch (entity)
    {
    case ET_GLOBAL_CPU_LOAD:
        return (this->currentGlobalCPULoadData->getMin());
        break;
    case ET_USED_MEMORY:
        return (this->currentUsedMemoryData->getMin());
        break;
    case ET_GLOBAL_CPU_TEMPERATURE:
        return (this->currentGlobalCPUTemperatureData->getMin());
        break;
    case ET_NETWORK_BANDWITH_DOWNLOAD_SPEED:
        return (this->currentNetworkDownloadUsedBandwithData->getMin());
        break;
    case ET_NETWORK_BANDWITH_UPLOAD_SPEED:
        return (this->currentNetworkUploadUsedBandwithData->getMin());
        break;
    default:
        return (false);
        break;
    }
}

uint64_t Source::getMax(EntityType entity) const
{
    switch (entity)
    {
    case ET_GLOBAL_CPU_LOAD:
        return (this->currentGlobalCPULoadData->getMax());
        break;
    case ET_USED_MEMORY:
        return (this->currentUsedMemoryData->getMax());
        break;
    case ET_GLOBAL_CPU_TEMPERATURE:
        return (this->currentGlobalCPUTemperatureData->getMax());
        break;
    case ET_NETWORK_BANDWITH_DOWNLOAD_SPEED:
        return (this->currentNetworkDownloadUsedBandwithData->getMax());
        break;
    case ET_NETWORK_BANDWITH_UPLOAD_SPEED:
        return (this->currentNetworkUploadUsedBandwithData->getMax());
        break;
    default:
        return (false);
        break;
    }
}

uint64_t Source::getPreviousValue(EntityType entity) const
{
    switch (entity)
    {
    case ET_GLOBAL_CPU_LOAD:
        return (this->currentGlobalCPULoadData->getPreviousValue());
        break;
    case ET_USED_MEMORY:
        return (this->currentUsedMemoryData->getPreviousValue());
        break;
    case ET_GLOBAL_CPU_TEMPERATURE:
        return (this->currentGlobalCPUTemperatureData->getPreviousValue());
        break;
    case ET_NETWORK_BANDWITH_DOWNLOAD_SPEED:
        return (this->currentNetworkDownloadUsedBandwithData->getPreviousValue());
        break;
    case ET_NETWORK_BANDWITH_UPLOAD_SPEED:
        return (this->currentNetworkUploadUsedBandwithData->getPreviousValue());
        break;
    default:
        return (false);
        break;
    }
}

uint64_t Source::getCurrentValue(EntityType entity) const
{
    switch (entity)
    {
    case ET_GLOBAL_CPU_LOAD:
        return (this->currentGlobalCPULoadData->getCurrentValue());
        break;
    case ET_USED_MEMORY:
        return (this->currentUsedMemoryData->getCurrentValue());
        break;
    case ET_GLOBAL_CPU_TEMPERATURE:
        return (this->currentGlobalCPUTemperatureData->getCurrentValue());
        break;
    case ET_NETWORK_BANDWITH_DOWNLOAD_SPEED:
        return (this->currentNetworkDownloadUsedBandwithData->getCurrentValue());
        break;
    case ET_NETWORK_BANDWITH_UPLOAD_SPEED:
        return (this->currentNetworkUploadUsedBandwithData->getCurrentValue());
        break;
    default:
        return (false);
        break;
    }
}

uint64_t Source::getCurrentGlobalCPULoad(void) const
{
    return (this->currentGlobalCPULoadData->getCurrentValue());
}

uint64_t Source::getTotalMemory(void) const
{
    return (this->currentUsedMemoryData->getMax());
}

uint64_t Source::getUsedMemory(void) const
{
    return (this->currentUsedMemoryData->getCurrentValue());
}

uint64_t Source::getCurrentGlobalCPUTemperature(void) const
{
    return (this->currentGlobalCPUTemperatureData->getCurrentValue());
}

uint64_t Source::getTotalNetworkDownloadBandwidth(void) const
{
    return (this->currentNetworkDownloadUsedBandwithData->getMax());
}

uint64_t Source::getCurrentNetworkDownloadUsedBandwidth(void) const
{
    return (this->currentNetworkDownloadUsedBandwithData->getCurrentValue());
}

uint64_t Source::getTotalNetworkUploadBandwidth(void) const
{
    return (this->currentNetworkUploadUsedBandwithData->getMax());
}

uint64_t Source::getCurrentNetworkUploadUsedBandwidth(void) const
{
    return (this->currentNetworkUploadUsedBandwithData->getCurrentValue());
}
