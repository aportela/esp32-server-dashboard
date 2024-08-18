#include "DummySource.hpp"
#include <Arduino.h>

DummySource::DummySource(SourceData *sourceData) : Source(sourceData)
{
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
    this->lastEllapsedMillis = millis();
    this->sourceData->setTotalMemory(17044639744); // 16 Gbytes
    this->sourceData->setUsedMemory(4261159936, this->lastEllapsedMillis);
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
        float fCurrent = 0.0f;
        uint64_t current = 0;
        uint64_t change = 0;

        uint8_t rnd = random(0, 100);
        if (rnd > 90)
        {
            fCurrent = random(MIN_CPU_LOAD, MAX_CPU_LOAD * 100);
        }
        else if (rnd > 50)
        {
            fCurrent = random(MIN_CPU_LOAD, (MAX_CPU_LOAD / 5) * 100);
        }
        else
        {
            fCurrent = random(MIN_CPU_LOAD, (MAX_CPU_LOAD / 10) * 100);
        }
        fCurrent /= 100.0f;
        this->sourceData->setCurrentCPULoad(fCurrent, currentMillis);

        current = this->sourceData->getUsedMemory();
        change = random(10485760, this->sourceData->getTotalMemory() / 1024);
        if (random(0, 20) % 2 == 0)
        {
            if (current < this->sourceData->getTotalMemory() - change)
            {
                current += change;
            }
        }
        else if (current > change)
        {
            current -= change;
        }
        this->sourceData->setUsedMemory(current, currentMillis);

        SourceDataQueueCPUTemperatureValue cpuTemperatureData = this->sourceData->getCurrentCPUTemperature();
        if (random(0, 20) % 2 == 0)
        {
            if (cpuTemperatureData.celsious < this->sourceData->getMaxCPUTemperature())
            {
                cpuTemperatureData.celsious++;
            }
        }
        else if (cpuTemperatureData.celsious > (this->sourceData->getMaxCPUTemperature() / 5))
        {
            cpuTemperatureData.celsious--;
        }
        this->sourceData->setCurrentCPUTemperature(cpuTemperatureData.celsious, currentMillis);

        this->sourceData->setCurrentUptimeSeconds(millis() / 1000, currentMillis);

        this->sourceData->setCurrentTotalNetworkDownloaded(this->sourceData->getCurrentTotalNetworkDownloaded() + random(1024, random(0, 100) > 90 ? 1048576 : 104857), currentMillis);

        this->sourceData->setCurrentTotalNetworkUploaded(this->sourceData->getCurrentTotalNetworkUploaded() + random(1024, random(0, 100) > 90 ? 1048576 : 104857), currentMillis);

        this->lastEllapsedMillis = currentMillis;
    }
}
