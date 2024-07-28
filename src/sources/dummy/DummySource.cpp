#include "DummySource.hpp"
#include <Arduino.h>

DummySource::DummySource(void) : Source()
{

    this->currentCPULoad = new EntityData(MIN_CPU_LOAD, MAX_CPU_LOAD);
    this->currentCPULoad->setCurrent(MIN_CPU_LOAD, millis());

    this->currentMemory = new EntityData(MIN_MEMORY, MAX_MEMORY);
    this->currentMemory->setCurrent(MIN_MEMORY, millis());

    this->currentCPUTemperature = new EntityData(MIN_CPU_TEMPERATURE, MAX_CPU_TEMPERATURE);
    this->currentCPUTemperature->setCurrent(MIN_CPU_TEMPERATURE, millis());

    this->currentNetworkDownloadBandwith = new EntityData(MIN_NETWORK_DOWNLOAD_BANDWITH, MAX_NETWORK_DOWNLOAD_BANDWITH);
    this->currentNetworkDownloadBandwith->setCurrent(MIN_NETWORK_DOWNLOAD_BANDWITH, millis());

    this->currentNetworkUploadBandwith = new EntityData(MIN_NETWORK_UPLOAD_BANDWITH, MAX_NETWORK_UPLOAD_BANDWITH);
    this->currentNetworkUploadBandwith->setCurrent(MIN_NETWORK_UPLOAD_BANDWITH, millis());
}

DummySource::~DummySource()
{
}

bool inc = false;

uint64_t DummySource::getCurrentCPULoad(void)
{
    uint64_t current = this->currentCPULoad->getCurrent();
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
    this->currentCPULoad->setCurrent(current, millis());
    return (current);
}

uint64_t DummySource::getTotalMemory(void)
{
    return (this->currentMemory->getMax());
}

uint64_t DummySource::getUsedMemory(void)
{
    uint64_t current = this->currentMemory->getCurrent();
    const uint64_t change = 1000000000;
    if (random(0, 20) % 2 == 0)
    {
        if (current < MAX_MEMORY - change)
        {
            current += change;
        }
    }
    else if (current > MIN_MEMORY + change)
    {
        current -= change;
    }
    this->currentMemory->setCurrent(current, millis());
    return (current);
}

uint64_t DummySource::getCurrentCPUTemperature(void)
{
    uint64_t current = this->currentCPUTemperature->getCurrent();
    if (random(0, 20) % 2 == 0)
    {
        if (current < MAX_CPU_TEMPERATURE)
        {
            current++;
        }
    }
    else if (current > (MAX_CPU_TEMPERATURE / 5))
    {
        current--;
    }
    this->currentCPUTemperature->setCurrent(current, millis());
    return (current);
}

uint64_t DummySource::getTotalNetworkDownloadBandwith(void)
{
    return (this->currentNetworkDownloadBandwith->getMax());
}

uint64_t DummySource::getUsedNetworkDownloadBandwith(void)
{
    uint64_t current = this->currentNetworkDownloadBandwith->getCurrent();
    const uint64_t change = random(10000, 5000000);
    if (random(0, 20) % 2 == 0)
    {
        if (current < MAX_NETWORK_DOWNLOAD_BANDWITH - change)
        {
            current += change;
        }
    }
    else if (current > MIN_NETWORK_DOWNLOAD_BANDWITH + change)
    {
        current -= change;
    }
    this->currentNetworkDownloadBandwith->setCurrent(current, millis());
    return (current);
}

uint64_t DummySource::getTotalNetworkUploadBandwith(void)
{
    return (this->currentNetworkUploadBandwith->getMax());
}

uint64_t DummySource::getUsedNetworkUploadBandwith(void)
{
    uint64_t current = this->currentNetworkUploadBandwith->getCurrent();
    const uint64_t change = random(500000, 10000000);
    if (random(0, 20) % 2 == 0)
    {
        if (current < MAX_NETWORK_UPLOAD_BANDWITH - change)
        {
            current += change;
        }
    }
    else if (current > MIN_NETWORK_UPLOAD_BANDWITH + change)
    {
        current -= change;
    }
    this->currentNetworkUploadBandwith->setCurrent(current, millis());
    return (current);
}
