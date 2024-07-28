#include "DummySource.hpp"
#include <Arduino.h>

#define MIN_CPU_LOAD 0
#define MAX_CPU_LOAD 100

#define MIN_MEMORY 0
#define MAX_MEMORY 32000000000 // 32 GB

#define MIN_CPU_TEMPERATURE 0
#define MAX_CPU_TEMPERATURE 100 // Celsious

#define MIN_NETWORK_DOWNLOAD_BANDWITH 0
#define MAX_NETWORK_DOWNLOAD_BANDWITH 512000000 // Mbytes

#define MIN_NETWORK_UPLOAD_BANDWITH 0
#define MAX_NETWORK_UPLOAD_BANDWITH 512000000 // Mbytes

DummySource::DummySource(void) : Source()
{
    this->currentGlobalCPULoadData->setMin(MIN_CPU_LOAD);
    this->currentGlobalCPULoadData->setMax(MAX_CPU_LOAD);
    this->currentGlobalCPULoadData->setCurrentValue(MIN_CPU_LOAD, millis());

    this->currentUsedMemoryData->setMin(MIN_MEMORY);
    this->currentUsedMemoryData->setMax(MAX_MEMORY);
    this->currentUsedMemoryData->setCurrentValue(MIN_MEMORY, millis());

    this->currentGlobalCPUTemperatureData->setMin(MIN_CPU_TEMPERATURE);
    this->currentGlobalCPUTemperatureData->setMax(MAX_CPU_TEMPERATURE);
    this->currentGlobalCPUTemperatureData->setCurrentValue(MIN_CPU_TEMPERATURE, millis());

    this->currentNetworkDownloadUsedBandwithData->setMin(MIN_NETWORK_DOWNLOAD_BANDWITH);
    this->currentNetworkDownloadUsedBandwithData->setMax(MAX_NETWORK_DOWNLOAD_BANDWITH);
    this->currentNetworkDownloadUsedBandwithData->setCurrentValue(MIN_NETWORK_DOWNLOAD_BANDWITH, millis());

    this->currentNetworkUploadUsedBandwithData->setMin(MIN_NETWORK_UPLOAD_BANDWITH);
    this->currentNetworkUploadUsedBandwithData->setMax(MAX_NETWORK_UPLOAD_BANDWITH);
    this->currentNetworkUploadUsedBandwithData->setCurrentValue(MIN_NETWORK_UPLOAD_BANDWITH, millis());
    // init random seed
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
}

DummySource::~DummySource()
{
}

void DummySource::refresh(void)
{
    uint64_t current = 0;
    uint64_t change = 0;
    uint64_t currentMillis = millis();

    // cpu load
    current = this->currentGlobalCPULoadData->getCurrentValue();
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
    if (this->inc)
    {
        if (current < MAX_CPU_LOAD)
        {
            current++;
        }
        else
        {
            this->inc = !this->inc;
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
            this->inc = !this->inc;
        }
    }
    this->currentGlobalCPULoadData->setCurrentValue(current, currentMillis);

    /*
    current = this->currentUsedMemoryData->getCurrentValue();
    change = 1000000000;
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
    this->currentUsedMemoryData->setCurrentValue(current, currentMillis);

    current = this->currentGlobalCPUTemperatureData->getCurrentValue();
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
    this->currentGlobalCPUTemperatureData->setCurrentValue(current, currentMillis);

    current = this->currentNetworkDownloadUsedBandwithData->getCurrentValue();
    change = random(10000, 5000000);
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
    this->currentNetworkDownloadUsedBandwithData->setCurrentValue(current, currentMillis);

    current = this->currentNetworkUploadUsedBandwithData->getCurrentValue();
    change = random(500000, 10000000);
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
    this->currentNetworkUploadUsedBandwithData->setCurrentValue(current, currentMillis);
    */
}
