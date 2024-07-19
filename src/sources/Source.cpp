#include "Source.hpp"
#include <Arduino.h>

Source::Source()
{
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
}

Source::~Source()
{
    if (this->currentCPULoad != nullptr)
    {
        delete this->currentCPULoad;
        this->currentCPULoad = nullptr;
    }
    if (this->currentMemory != nullptr)
    {
        delete this->currentMemory;
        this->currentMemory = nullptr;
    }
    if (this->currentCPUTemperature != nullptr)
    {
        delete this->currentCPUTemperature;
        this->currentCPUTemperature = nullptr;
    }
    if (this->currentNetworkDownloadBandwith != nullptr)
    {
        delete this->currentNetworkDownloadBandwith;
        this->currentNetworkDownloadBandwith = nullptr;
    }
    if (this->currentNetworkUploadBandwith != nullptr)
    {
        delete this->currentNetworkUploadBandwith;
        this->currentNetworkUploadBandwith = nullptr;
    }
}

bool Source::changed(EntitySource entity)
{
    switch (entity)
    {
    case CPU_LOAD:
        return (this->currentCPULoad->changed());
        break;
    case MEMORY:
        return (this->currentMemory->changed());
        break;
    case CPU_TEMPERATURE:
        return (this->currentCPUTemperature->changed());
        break;
    case NETWORK_BANDWITH_DOWNLOAD:
        return (this->currentNetworkDownloadBandwith->changed());
        break;
    case NETWORK_BANDWITH_UPLOAD:
        return (this->currentNetworkUploadBandwith->changed());
        break;
    default:
        return (true);
        break;
    }
}

uint64_t Source::getPrevious(EntitySource entity)
{
    switch (entity)
    {
    case CPU_LOAD:
        return (this->currentCPULoad->getPrevious());
        break;
    case MEMORY:
        return (this->currentMemory->getPrevious());
        break;
    case CPU_TEMPERATURE:
        return (this->currentCPUTemperature->getPrevious());
        break;
    case NETWORK_BANDWITH_DOWNLOAD:
        return (this->currentNetworkDownloadBandwith->getPrevious());
        break;
    case NETWORK_BANDWITH_UPLOAD:
        return (this->currentNetworkUploadBandwith->getPrevious());
        break;
    default:
        return (0);
        break;
    }
}

uint64_t Source::getCurrent(EntitySource entity)
{
    switch (entity)
    {
    case CPU_LOAD:
        return (this->currentCPULoad->getCurrent());
        break;
    case MEMORY:
        return (this->currentMemory->getCurrent());
        break;
    case CPU_TEMPERATURE:
        return (this->currentCPUTemperature->getCurrent());
        break;
    case NETWORK_BANDWITH_DOWNLOAD:
        return (this->currentNetworkDownloadBandwith->getCurrent());
        break;
    case NETWORK_BANDWITH_UPLOAD:
        return (this->currentNetworkUploadBandwith->getCurrent());
        break;
    default:
        return (0);
        break;
    }
}

uint64_t Source::getPreviousCPULoad(void)
{
    return (this->currentCPULoad->getPrevious());
}

uint64_t Source::getPreviousUsedMemory(void)
{
    return (this->currentMemory->getPrevious());
}

uint64_t Source::getPreviousCPUTemperature(void)
{
    return (this->currentCPUTemperature->getPrevious());
}

uint64_t Source::getPreviousUsedNetworkDownloadBandwith(void)
{
    return (this->currentNetworkDownloadBandwith->getPrevious());
}

uint64_t Source::getPreviousUsedNetworkUploadBandwith(void)
{
    return (this->currentNetworkUploadBandwith->getPrevious());
}

void Source::refresh(void)
{
    this->getCurrentCPULoad();
    this->getUsedMemory();
    this->getCurrentCPUTemperature();
    this->getUsedNetworkDownloadBandwith();
    this->getUsedNetworkUploadBandwith();
}