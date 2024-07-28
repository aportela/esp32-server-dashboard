#include "LGFXScreenDashboardResume.hpp"
#include "../SizesAndOffsets-320x240.hpp"
#include "../../utils/WifiManager.hpp"
#include "../../utils/FPS.hpp"

#define METER_GRAPH_WIDTH 195
#define METER_GRAPH_HEIGHT 30

#define METER_BG_COLOR TFT_BLACK

#define DEBUG_SPRITE_WIDTH DISPLAY_WIDTH - 20
#define DEBUG_SPRITE_HEIGHT 13
#define DEBUG_SPRITE_BACKGROUND TFT_BLACK

LGFXScreenDashboardResume::LGFXScreenDashboardResume(LovyanGFX *display, ISource *source) : LGFXScreen(display)
{
    if (display != nullptr)
    {
        this->cpuLoadMeter = new LGFXMeter(this->parentDisplay, ET_CPU_LOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 0, METER_BG_COLOR, "CPU LOAD");
        this->cpuLoadMeter->setMin(0);
        this->cpuLoadMeter->setMax(100);
        this->memoryLoadMeter = new LGFXMeter(this->parentDisplay, ET_USED_MEMORY, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 42, METER_BG_COLOR, "MEMORY");
        this->memoryLoadMeter->setMin(0);
        this->memoryLoadMeter->setMax(34231410688); // 32 Gb
        this->cpuTemperatureLoadMeter = new LGFXMeter(this->parentDisplay, ET_GLOBAL_CPU_TEMPERATURE, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 84, METER_BG_COLOR, "CPU TEMP");
        this->cpuTemperatureLoadMeter->setMin(0);
        this->cpuTemperatureLoadMeter->setMax(100);
        this->networkDownloadBandwithLoadMeter = new LGFXMeter(this->parentDisplay, ET_NETWORK_BANDWITH_DOWNLOAD_SPEED, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 126, METER_BG_COLOR, "WAN DOWNLOAD");
        this->networkDownloadBandwithLoadMeter->setMin(0);
        this->networkDownloadBandwithLoadMeter->setMax(536870912); // 512 Mb
        this->networkUploadBandwithLoadMeter = new LGFXMeter(this->parentDisplay, ET_NETWORK_BANDWITH_UPLOAD_SPEED, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 168, METER_BG_COLOR, "WAN UPLOAD");

        this->debugSprite = new lgfx::LGFX_Sprite(this->parentDisplay);
        this->debugSprite->createSprite(DEBUG_SPRITE_WIDTH, DEBUG_SPRITE_HEIGHT);
        this->debugSprite->setFont(&fonts::Font2);

        this->refresh(true);
    }
    if (source != nullptr)
    {
        this->currentSource = source;
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
    if (this->debugSprite != nullptr)
    {
        delete this->debugSprite;
        this->debugSprite = nullptr;
    }
    if (this->parentDisplay != nullptr)
    {
        this->parentDisplay = nullptr;
    }
    if (this->currentSource != nullptr)
    {
        this->currentSource = nullptr;
    }
}

void LGFXScreenDashboardResume::refreshDebug(uint16_t xOffset, uint16_t yOffset, int32_t wifiSignalStrength)
{
    FPS::loop();

    this->debugSprite->fillSprite(DEBUG_SPRITE_BACKGROUND);
    this->debugSprite->setTextSize(1);
    this->debugSprite->setTextColor(TFT_WHITE, TFT_BLACK);
    this->debugSprite->setCursor(0, 0);
    char timeString[50] = {'\0'};
    // convertmillisToHumanStr(millis(), timeString, sizeof(timeString));
    this->debugSprite->printf("Runtime: %s - FPS: %03u - Wifi: %03ddBm", "00 secs", FPS::getFPS(), WifiManager::getSignalStrength());
    this->debugSprite->pushSprite(xOffset, yOffset);
}

void LGFXScreenDashboardResume::refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        uint64_t currentMillis = millis();

        if (force || this->currentSource->changed(CPU_LOAD, currentMillis))
        {
            // cpuLoadMeter->refresh(this->currentSource->getCurrentCPULoad());
        }
        if (force || this->currentSource->changed(MEMORY, currentMillis))
        {
            // memoryLoadMeter->refresh(this->currentSource->getUsedMemory());
        }
        if (force || this->currentSource->changed(CPU_TEMPERATURE, currentMillis))
        {
            // cpuTemperatureLoadMeter->refresh(this->currentSource->getCurrentCPUTemperature());
        }
        if (force || this->currentSource->changed(NETWORK_BANDWITH_DOWNLOAD, currentMillis))
        {
            // networkDownloadBandwithLoadMeter->refresh(this->currentSource->getUsedNetworkDownloadBandwith());
        }
        if (force || this->currentSource->changed(NETWORK_BANDWITH_UPLOAD, currentMillis))
        {
            // networkUploadBandwithLoadMeter->refresh(this->currentSource->getUsedNetworkUploadBandwith());
        }
        // TODO: current source == null WARNING on bottom
        this->refreshDebug(0, 210, WifiManager::getSignalStrength());
    }
}
