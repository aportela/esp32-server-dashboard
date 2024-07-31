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

DummySource::DummySource(SourceData *sourceData) : Source(sourceData)
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

    uint8_t rnd = random(0, 100);
    if (rnd > 90)
    {
        current = random(MIN_CPU_LOAD, MAX_CPU_LOAD);
    }
    else if (rnd > 50)
    {
        current = random(MIN_CPU_LOAD, MAX_CPU_LOAD / 5);
    }
    else
    {
        current = random(MIN_CPU_LOAD, MAX_CPU_LOAD / 10);
    }
    this->currentGlobalCPULoadData->setCurrentValue(current, currentMillis);

    this->sourceData->setCurrentCPULoad(current, millis());

    current = this->currentUsedMemoryData->getCurrentValue();
    change = this->currentUsedMemoryData->getMax() / 100;
    if (random(0, 20) % 2 == 0)
    {
        if (current < this->currentUsedMemoryData->getMax() - change)
        {
            current += change;
        }
    }
    else if (current > this->currentUsedMemoryData->getMin() + change)
    {
        current -= change;
    }
    this->currentUsedMemoryData->setCurrentValue(current, currentMillis);

    this->sourceData->setUsedMemory(current, millis());

    current = this->currentGlobalCPUTemperatureData->getCurrentValue();
    if (random(0, 20) % 2 == 0)
    {
        if (current < this->currentGlobalCPUTemperatureData->getMax())
        {
            current++;
        }
    }
    else if (current > (this->currentGlobalCPUTemperatureData->getMax() / 5))
    {
        current--;
    }
    this->currentGlobalCPUTemperatureData->setCurrentValue(current, currentMillis);

    this->sourceData->setCurrentCPUTemperature(current, millis());

    current = this->currentNetworkDownloadUsedBandwithData->getCurrentValue();
    change = random(1000, 5000000);
    if (random(0, 20) % 2 == 0)
    {
        if (current < this->currentNetworkDownloadUsedBandwithData->getMax() - change)
        {
            current += change;
        }
    }
    else if (current > this->currentNetworkDownloadUsedBandwithData->getMin() + change)
    {
        current -= change;
    }
    this->currentNetworkDownloadUsedBandwithData->setCurrentValue(current, currentMillis);

    this->sourceData->setCurrentTotalNetworkDownloaded(random(1000000, 5000000000), millis());

    current = this->currentNetworkUploadUsedBandwithData->getCurrentValue();
    change = random(500000, 10000000);
    if (random(0, 20) % 2 == 0)
    {
        if (current < this->currentNetworkUploadUsedBandwithData->getMax() - change)
        {
            current += change;
        }
    }
    else if (current > this->currentNetworkUploadUsedBandwithData->getMin() + change)
    {
        current -= change;
    }
    this->currentNetworkUploadUsedBandwithData->setCurrentValue(current, currentMillis);

    this->sourceData->setCurrentTotalNetworkUploaded(random(1000000, 5000000000), millis());
}
