#include "LGFXScreenDashboardResume.hpp"

#define METER_GRAPH_WIDTH 195
#define METER_GRAPH_HEIGHT 30

#define METER_BG_COLOR TFT_BLACK

LGFXScreenDashboardResume::LGFXScreenDashboardResume(LovyanGFX *display) : LGFXScreen(display)
{
    if (display != nullptr)
    {

        this->cpuLoadMeter = new LGFXMeter(this->parentDisplay, METER_ENTITY_CPU_LOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 0, METER_BG_COLOR, "CPU LOAD");
        this->cpuLoadMeter->setMin(0);
        this->cpuLoadMeter->setMax(100);
        this->memoryLoadMeter = new LGFXMeter(this->parentDisplay, METER_ENTITY_MEMORY, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 42, METER_BG_COLOR, "MEMORY");
        this->memoryLoadMeter->setMin(0);
        this->memoryLoadMeter->setMax(34231410688); // 32 Gb
        this->cpuTemperatureLoadMeter = new LGFXMeter(this->parentDisplay, METER_ENTITY_CPU_TEMPERATURE, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 84, METER_BG_COLOR, "CPU TEMP");
        this->cpuTemperatureLoadMeter->setMin(0);
        this->cpuTemperatureLoadMeter->setMax(100);
        this->networkDownloadBandwithLoadMeter = new LGFXMeter(this->parentDisplay, METER_ENTITY_NETWORK_BANDWITH_DOWNLOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 126, METER_BG_COLOR, "WAN DOWNLOAD");
        this->networkDownloadBandwithLoadMeter->setMin(0);
        this->networkDownloadBandwithLoadMeter->setMax(536870912); // 512 Mb
        this->networkUploadBandwithLoadMeter = new LGFXMeter(this->parentDisplay, METER_ENTITY_NETWORK_BANDWITH_UPLOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 168, METER_BG_COLOR, "WAN UPLOAD");
        this->refresh(true);
    }
}

LGFXScreenDashboardResume::~LGFXScreenDashboardResume()
{
    if (this->cpuLoadMeter != nullptr)
    {
        delete this->cpuLoadMeter;
        this->cpuLoadMeter = nullptr;
    }
    if (this->memoryLoadMeter != nullptr)
    {
        delete this->memoryLoadMeter;
        this->memoryLoadMeter = nullptr;
    }
    if (this->cpuTemperatureLoadMeter != nullptr)
    {
        delete this->cpuTemperatureLoadMeter;
        this->cpuTemperatureLoadMeter = nullptr;
    }
    if (this->networkDownloadBandwithLoadMeter != nullptr)
    {
        delete this->networkDownloadBandwithLoadMeter;
        this->networkDownloadBandwithLoadMeter = nullptr;
    }
    if (this->networkUploadBandwithLoadMeter != nullptr)
    {
        delete this->networkUploadBandwithLoadMeter;
        this->networkUploadBandwithLoadMeter = nullptr;
    }
    this->parentDisplay = nullptr;
}

void LGFXScreenDashboardResume::refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {

        /*
        if (refreshAlways || mqttTelegrafSRC->changed(CPU_LOAD, currentMillis))
        {
            cpuLoadMeter->refresh(mqttTelegrafSRC->getCurrentCPULoad());
            Serial.println("REFRESH CPU");
        }
        if (refreshAlways || mqttTelegrafSRC->changed(MEMORY, currentMillis))
        {
            memoryLoadMeter->refresh(mqttTelegrafSRC->getUsedMemory());
        }
        if (refreshAlways || mqttTelegrafSRC->changed(CPU_TEMPERATURE, currentMillis))
        {
            cpuTemperatureLoadMeter->refresh(mqttTelegrafSRC->getCurrentCPUTemperature());
        }
        if (refreshAlways || mqttTelegrafSRC->changed(NETWORK_BANDWITH_DOWNLOAD, currentMillis))
        {
            networkDownloadBandwithLoadMeter->refresh(mqttTelegrafSRC->getUsedNetworkDownloadBandwith());
        }
        if (refreshAlways || mqttTelegrafSRC->changed(NETWORK_BANDWITH_UPLOAD, currentMillis))
        {
            networkUploadBandwithLoadMeter->refresh(mqttTelegrafSRC->getUsedNetworkUploadBandwith());
        }
        screen->refreshDebug(0, 210, WifiManager::getSignalStrength());
        */
    }
}
