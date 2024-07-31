#include "LGFXScreenDashboardResume.hpp"
#include <inttypes.h>
#include "../SizesAndOffsets-320x240.hpp"
#include "../../utils/WifiManager.hpp"
#include "../../utils/FPS.hpp"
#include "../../EntityType.hpp"
#include "../../utils/Format.hpp"
#include "../../utils/Settings.hpp"

#define METER_GRAPH_WIDTH 195
#define METER_GRAPH_HEIGHT 30
#define METER_GRAPH_BG_COLOR TFT_BLACK

#define SCREEN_BOTTOM_COMMON_TEXTDATA_FONT &fonts::Font2
#define SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE 1
#define SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR TFT_WHITE
#define SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR TFT_BLACK
#define SCREEN_BOTTOM_COMMON_TEXTDATA_X_OFFSET 0
#define SCREEN_BOTTOM_COMMON_TEXTDATA_FIELD_FPS_X_OFFSET 33
#define SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET 226

LGFXScreenDashboardResume::LGFXScreenDashboardResume(LovyanGFX *display, Source *source, SourceData *sourceData) : LGFXScreen(display)
{
    if (source != nullptr)
    {
        this->currentSource = source;
    }
    if (sourceData != nullptr)
    {
        this->currentSourceData = sourceData;
    }
    if (display != nullptr)
    {
        this->cpuLoadMeter = new LGFXMeter(this->parentDisplay, ET_GLOBAL_CPU_LOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 0, METER_GRAPH_BG_COLOR, "CPU LOAD", 0, 100);
        this->memoryLoadMeter = new LGFXMeter(this->parentDisplay, ET_USED_MEMORY, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 42, METER_GRAPH_BG_COLOR, "MEMORY", 0, Settings::getTotalMemoryBytes());
        this->cpuTemperatureLoadMeter = new LGFXMeter(this->parentDisplay, ET_GLOBAL_CPU_TEMPERATURE, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 84, METER_GRAPH_BG_COLOR, "CPU TEMP", Settings::getMinCPUTemperature(), Settings::getMaxCPUTemperature());
        this->networkDownloadBandwithLoadMeter = new LGFXMeter(this->parentDisplay, ET_NETWORK_BANDWITH_DOWNLOAD_SPEED, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 126, METER_GRAPH_BG_COLOR, "WAN DOWNLOAD", 0, Settings::getMaxDownloadBandwidthBytes());
        this->networkUploadBandwithLoadMeter = new LGFXMeter(this->parentDisplay, ET_NETWORK_BANDWITH_UPLOAD_SPEED, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 168, METER_GRAPH_BG_COLOR, "WAN UPLOAD", 0, Settings::getMaxUploadBandwidthBytes());
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
    if (this->parentDisplay != nullptr)
    {
        this->parentDisplay = nullptr;
    }
    if (this->currentSource != nullptr)
    {
        this->currentSource = nullptr;
    }
}

void LGFXScreenDashboardResume::refreshBottomCommonData(bool forceDrawAll)
{
    FPS::loop();
    this->parentDisplay->setFont(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT);
    this->parentDisplay->setTextSize(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE);
    this->parentDisplay->setTextColor(SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR, SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR);
    this->parentDisplay->setCursor(SCREEN_BOTTOM_COMMON_TEXTDATA_X_OFFSET, SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET);
    this->parentDisplay->print("FPS: ");
    uint16_t currentFPS = FPS::getFPS();
    if (forceDrawAll || currentFPS != this->previousFPS)
    {
        this->parentDisplay->setFont(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT);
        this->parentDisplay->setTextSize(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE);
        this->parentDisplay->setTextColor(SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR, SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR);
        this->parentDisplay->setCursor(SCREEN_BOTTOM_COMMON_TEXTDATA_FIELD_FPS_X_OFFSET, SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET);
        this->parentDisplay->printf("%03u", currentFPS);
        this->previousFPS = currentFPS;
    }
    // TODO: current uptime & other data
}

void LGFXScreenDashboardResume::refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        if (this->currentSource != nullptr)
        {
            /*
            uint64_t currentCPUMillis = this->currentSourceData->getCurrentCPULoadTimestamp();
            if (force || currentCPUMillis > this->lastCPUTimestamp)
            {
                float currentCPULoad = this->currentSourceData->getCurrentCPULoad();
                this->cpuLoadMeter->refresh((uint64_t)currentCPULoad);
                this->lastCPUTimestamp = currentCPUMillis;
            }
            */
            uint64_t currentCPUMillis = this->currentSource->getCurrentTimestamp(ET_GLOBAL_CPU_LOAD);
            if (force || currentCPUMillis > this->lastCPUTimestamp)
            {
                this->cpuLoadMeter->refresh(this->currentSource->getCurrentGlobalCPULoad());
                this->lastCPUTimestamp = currentCPUMillis;
            }
            uint64_t currentMemoryMillis = this->currentSource->getCurrentTimestamp(ET_USED_MEMORY);
            if (force || currentMemoryMillis > this->lastMemoryTimestamp)
            {
                this->memoryLoadMeter->refresh(this->currentSource->getUsedMemory());
                this->lastMemoryTimestamp = currentMemoryMillis;
            }
            uint64_t currentCPUTemperatureMillis = this->currentSource->getCurrentTimestamp(ET_GLOBAL_CPU_TEMPERATURE);
            if (force || currentCPUTemperatureMillis > this->lastCPUTemperatureTimestamp)
            {
                this->cpuTemperatureLoadMeter->refresh(this->currentSource->getCurrentGlobalCPUTemperature());
                this->lastCPUTemperatureTimestamp = currentCPUTemperatureMillis;
            }
            uint64_t currentDownloadMillis = this->currentSource->getCurrentTimestamp(ET_NETWORK_BANDWITH_DOWNLOAD_SPEED);
            if (force || currentDownloadMillis > this->lastDownloadTimestamp)
            {
                this->networkDownloadBandwithLoadMeter->refresh(this->currentSource->getCurrentNetworkDownloadUsedBandwidth());
                this->lastDownloadTimestamp = currentDownloadMillis;
            }
            uint64_t currentUploadMillis = this->currentSource->getCurrentTimestamp(ET_NETWORK_BANDWITH_UPLOAD_SPEED);
            if (force || currentUploadMillis > this->lastUploadTimestamp)
            {
                this->networkUploadBandwithLoadMeter->refresh(this->currentSource->getCurrentNetworkUploadUsedBandwidth());
                this->lastUploadTimestamp = currentUploadMillis;
            }
        }
        this->refreshBottomCommonData(force);
    }
}
