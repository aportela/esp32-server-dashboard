#include "DummySource.hpp"
#include <Arduino.h>

DummySource::DummySource(SourceData *sourceData) : Source(sourceData)
{
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
    this->lastEllapsedMillis = millis();
    this->sourceData->SetCurrentUsedMemory(34359738368 / 10, 34359738368, this->lastEllapsedMillis); // total memory = 32 Gbytes
}

DummySource::~DummySource()
{
}

void DummySource::Refresh(uint16_t milliSeconds)
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
        this->sourceData->SetCurrentCPULoad(cpuLoadValue, currentMillis);

        SourceDataQueueUsedMemoryValue memoryData = this->sourceData->GetCurrentUsedMemory();
        uint64_t changedMemoryBytes = random(memoryData.totalBytes / 1024, memoryData.totalBytes / 20);
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
        this->sourceData->SetCurrentUsedMemory(memoryData.usedBytes, memoryData.totalBytes, currentMillis);

        SourceDataQueueCPUTemperatureValue cpuTemperatureData = this->sourceData->GetCurrentCPUTemperature();
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
        this->sourceData->SetCurrentCPUTemperature(cpuTemperatureData.celsious, currentMillis);

        this->sourceData->SetCurrentUptime(currentMillis / 1000, currentMillis);

        SourceDataQueueNetworkingLimitsValue netLimits = this->sourceData->GetNetworkLimits();

        SourceDataQueueNetworkingValue netDownData = this->sourceData->GetCurrentNetworkDownload();
        if (netLimits.byteDownloadLimit == 0)
        {
            netLimits.byteDownloadLimit = 1048576;
        }
        this->sourceData->SetCurrentNetworkDownload(netDownData.totalBytesTransfered + (random(1024, random(0, 100) > 95 ? netLimits.byteDownloadLimit : netLimits.byteDownloadLimit / 200)), currentMillis);

        SourceDataQueueNetworkingValue netUpData = this->sourceData->GetCurrentNetworkUpload();
        if (netLimits.byteUploadLimit == 0)
        {
            netLimits.byteUploadLimit = 1048576;
        }
        this->sourceData->SetCurrentNetworkUpload(netUpData.totalBytesTransfered + (random(1024, random(0, 100) > 95 ? netLimits.byteUploadLimit : netLimits.byteUploadLimit / 200)), currentMillis);

        this->lastEllapsedMillis = currentMillis;
    }
}
