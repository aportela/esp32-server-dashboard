#include "DummySource.hpp"
#include <Arduino.h>

DummySource::DummySource(void)
{
    randomSeed(analogRead(0) ^ (micros() * esp_random()));

    this->currentCPULoad = new SourceDataCPULoad(MIN_CPU_LOAD, MAX_CPU_LOAD);
    this->currentCPULoad->setCurrent(random(MIN_CPU_LOAD, MAX_CPU_LOAD));

    this->currentMemory = new SourceDataMemory(MIN_MEMORY, MAX_MEMORY);
    this->currentMemory->setCurrent(random(MIN_MEMORY, MAX_MEMORY));

    this->currentCPUTemperature = new SourceDataCPUTemperature(MIN_CPU_TEMPERATURE, MAX_CPU_TEMPERATURE);
    this->currentCPUTemperature->setCurrent(random(MIN_CPU_TEMPERATURE, MAX_CPU_TEMPERATURE));

    this->currentMemory = new SourceDataMemory(MIN_MEMORY, MAX_MEMORY);
    this->currentMemory->setCurrent(random(MIN_MEMORY, MAX_MEMORY));

    this->currentNetworkDownloadBandwith = new SourceDataNetworkUsedBandwith(MIN_NETWORK_DOWNLOAD_BANDWITH, MAX_NETWORK_DOWNLOAD_BANDWITH);
    this->currentNetworkDownloadBandwith->setCurrent(0);
    // this->currentNetworkDownloadBandwith->setCurrent(MAX_NETWORK_DOWNLOAD_BANDWITH / (random(1, 5) * 2));

    this->currentNetworkUploadBandwith = new SourceDataNetworkUsedBandwith(MIN_NETWORK_UPLOAD_BANDWITH, MAX_NETWORK_UPLOAD_BANDWITH);
    this->currentNetworkUploadBandwith->setCurrent(MAX_NETWORK_DOWNLOAD_BANDWITH / (random(1, 5) * 2));
}

DummySource::~DummySource()
{
    delete this->currentCPULoad;
    this->currentCPULoad = nullptr;
    delete this->currentMemory;
    this->currentMemory = nullptr;
    delete this->currentCPUTemperature;
    this->currentCPUTemperature = nullptr;
    delete this->currentNetworkDownloadBandwith;
    this->currentNetworkDownloadBandwith = nullptr;
    delete this->currentNetworkUploadBandwith;
    this->currentNetworkUploadBandwith = nullptr;
}

uint8_t DummySource::getCurrentCPULoad(void)
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

uint8_t DummySource::getCurrentCPUTemperature(void)
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
