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
