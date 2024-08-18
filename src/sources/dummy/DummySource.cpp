#include "DummySource.hpp"
#include <Arduino.h>

DummySource::DummySource(SourceData *sourceData) : Source(sourceData)
{
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
    this->lastEllapsedMillis = millis();
    this->sourceData->setCurrentUsedMemory(4261159936, 17044639744, this->lastEllapsedMillis);
    this->sourceData->setNetworkDownloadBandwidthLimit(73125000);
    this->sourceData->setNetworkUploadBandwidthLimit(73125000);
}

DummySource::~DummySource()
{
}

void DummySource::refresh(uint16_t milliSeconds)
{
    uint64_t currentMillis = millis();
    bool allowRefresh = milliSeconds == 0 || ((currentMillis - this->lastEllapsedMillis) >= milliSeconds);
    if (allowRefresh)
    {
        float cpuLoadValue = 0.0f;
        uint8_t rnd = random(0, 100);
        if (rnd > 90)
        {
            cpuLoadValue = random(MIN_CPU_LOAD, MAX_CPU_LOAD * 100);
        }
        else if (rnd > 50)
        {
            cpuLoadValue = random(MIN_CPU_LOAD, (MAX_CPU_LOAD / 5) * 100);
        }
        else
        {
            cpuLoadValue = random(MIN_CPU_LOAD, (MAX_CPU_LOAD / 10) * 100);
        }
        cpuLoadValue /= 100.0f;
        this->sourceData->setCurrentCPULoad(cpuLoadValue, currentMillis);

        SourceDataQueueUsedMemoryValue memoryData = this->sourceData->getCurrentUsedMemory();
        uint64_t changedMemoryBytes = random(10485760, memoryData.totalBytes / 1024);
        if (random(0, 20) % 2 == 0)
        {
            if (memoryData.usedBytes < memoryData.totalBytes - changedMemoryBytes)
            {
                memoryData.usedBytes += changedMemoryBytes;
            }
        }
        else if (memoryData.usedBytes > changedMemoryBytes)
        {
            memoryData.usedBytes -= changedMemoryBytes;
        }
        this->sourceData->setCurrentUsedMemory(memoryData.usedBytes, memoryData.totalBytes, currentMillis);

        SourceDataQueueCPUTemperatureValue cpuTemperatureData = this->sourceData->getCurrentCPUTemperature();
        if (random(0, 20) % 2 == 0)
        {
            if (cpuTemperatureData.celsious < MAX_CPU_TEMPERATURE)
            {
                cpuTemperatureData.celsious++;
            }
        }
        else if (cpuTemperatureData.celsious > (MAX_CPU_TEMPERATURE / 5))
        {
            cpuTemperatureData.celsious--;
        }
        this->sourceData->setCurrentCPUTemperature(cpuTemperatureData.celsious, currentMillis);

        this->sourceData->setCurrentUptime(currentMillis / 1000, currentMillis);

        SourceDataQueueNetworkingValue netDownData = this->sourceData->getCurrentNetworkDownload();
        this->sourceData->setCurrentNetworkDownload(netDownData.totalBytesTransfered + (random(1024, random(0, 100) > 90 ? 1048576 : 104857)), currentMillis);

        SourceDataQueueNetworkingValue netUpData = this->sourceData->getCurrentNetworkUpload();
        this->sourceData->setCurrentNetworkUpload(netUpData.totalBytesTransfered + (random(1024, random(0, 100) > 90 ? 1048576 : 104857)), currentMillis);

        this->lastEllapsedMillis = currentMillis;
    }
}
