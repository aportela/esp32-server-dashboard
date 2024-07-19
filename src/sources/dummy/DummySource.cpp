#include "DummySource.hpp"
#include <Arduino.h>

DummySource::DummySource(void) : Source()
{

    this->currentCPULoad = new SourceData(MIN_CPU_LOAD, MAX_CPU_LOAD);
    this->currentCPULoad->setCurrent(random(MIN_CPU_LOAD, MAX_CPU_LOAD));

    this->currentMemory = new SourceData(MIN_MEMORY, MAX_MEMORY);
    this->currentMemory->setCurrent(random(MIN_MEMORY, MAX_MEMORY));

    this->currentCPUTemperature = new SourceData(MIN_CPU_TEMPERATURE, MAX_CPU_TEMPERATURE);
    this->currentCPUTemperature->setCurrent(random(MIN_CPU_TEMPERATURE, MAX_CPU_TEMPERATURE));

    this->currentMemory = new SourceData(MIN_MEMORY, MAX_MEMORY);
    this->currentMemory->setCurrent(random(MIN_MEMORY, MAX_MEMORY));

    this->currentNetworkDownloadBandwith = new SourceData(MIN_NETWORK_DOWNLOAD_BANDWITH, MAX_NETWORK_DOWNLOAD_BANDWITH);
    this->currentNetworkDownloadBandwith->setCurrent(0);
    // this->currentNetworkDownloadBandwith->setCurrent(MAX_NETWORK_DOWNLOAD_BANDWITH / (random(1, 5) * 2));

    this->currentNetworkUploadBandwith = new SourceData(MIN_NETWORK_UPLOAD_BANDWITH, MAX_NETWORK_UPLOAD_BANDWITH);
    this->currentNetworkUploadBandwith->setCurrent(MAX_NETWORK_DOWNLOAD_BANDWITH / (random(1, 5) * 2));
}

DummySource::~DummySource()
{
}

bool inc = false;

uint64_t DummySource::getCurrentCPULoad(void)
{
    uint8_t current = this->currentCPULoad->getCurrent();
    if (random(0, 20) % 2 == 0)
    {
        if (current < MAX_CPU_LOAD)
        {
            current++;
        }
    }
    else if (current > MIN_CPU_LOAD)
    {
        current--;
    }
    if (inc)
    {
        if (current < MAX_CPU_LOAD)
        {
            current++;
        }
        else
        {
            inc = !inc;
        }
    }
    else
    {
        if (current > MIN_CPU_LOAD)
        {
            current--;
        }
        else
        {
            inc = !inc;
        }
    }
    this->currentCPULoad->setCurrent(current);
    return (current);
}

uint64_t DummySource::getTotalMemory(void)
{
    return (this->currentMemory->getMax());
}

uint64_t DummySource::getUsedMemory(void)
{
    uint8_t current = this->currentMemory->getCurrent();
    if (random(0, 20) % 2 == 0)
    {
        if (current < MAX_MEMORY)
        {
            current++;
        }
    }
    else if (current > MIN_MEMORY)
    {
        current--;
    }
    this->currentMemory->setCurrent(current);
    return (current);
}

uint64_t DummySource::getCurrentCPUTemperature(void)
{
    uint8_t current = this->currentCPUTemperature->getCurrent();
    if (random(0, 20) % 2 == 0)
    {
        if (current < MAX_CPU_TEMPERATURE)
        {
            current++;
        }
    }
    else if (current > MIN_CPU_TEMPERATURE)
    {
        current--;
    }
    this->currentCPUTemperature->setCurrent(current);
    return (current);
}

uint64_t DummySource::getTotalNetworkDownloadBandwith(void)
{
    return (this->currentNetworkDownloadBandwith->getMax());
}

uint64_t DummySource::getUsedNetworkDownloadBandwith(void)
{
    uint64_t current = this->currentNetworkDownloadBandwith->getCurrent();
    if (current + 500000 < MAX_NETWORK_DOWNLOAD_BANDWITH)
    {
        current += 1000000;
    }
    /*
    if (random(0, 20) % 2 == 0)
    {
        if (current + 5000000 < MAX_NETWORK_DOWNLOAD_BANDWITH)
        {
            current += 5000000;
        }
    }
    else if (current - 5000000 > MIN_NETWORK_DOWNLOAD_BANDWITH)
    {
        current -= 5000000;
    }
    */
    this->currentNetworkDownloadBandwith->setCurrent(current);
    return (current);
}

uint64_t DummySource::getTotalNetworkUploadBandwith(void)
{
    return (this->currentNetworkUploadBandwith->getMax());
}

uint64_t DummySource::getUsedNetworkUploadBandwith(void)
{
    uint64_t current = this->currentNetworkUploadBandwith->getCurrent();
    if (random(0, 20) % 2 == 0)
    {
        if (current + 5000000 < MAX_NETWORK_UPLOAD_BANDWITH)
        {
            current += 5000000;
        }
    }
    else if (current - 5000000 > MIN_NETWORK_UPLOAD_BANDWITH)
    {
        current -= 5000000;
    }
    this->currentNetworkUploadBandwith->setCurrent(current);
    return (current);
}
