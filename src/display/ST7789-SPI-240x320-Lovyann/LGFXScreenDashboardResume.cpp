#include "LGFXScreenDashboardResume.hpp"
#include <inttypes.h>
#include "../SizesAndOffsets-320x240.hpp"
#include "../../utils/WifiManager.hpp"
#include "../../utils/FPS.hpp"
#include "../../EntityType.hpp"
#include "../../utils/Format.hpp"

#define METER_GRAPH_WIDTH 195
#define METER_GRAPH_HEIGHT 30
#define METER_GRAPH_BG_COLOR TFT_BLACK

#define SCREEN_BOTTOM_COMMON_TEXTDATA_FONT &fonts::Font2
#define SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE 1
#define SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR TFT_WHITE
#define SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR TFT_BLACK
#define SCREEN_BOTTOM_COMMON_TEXTDATA_X_OFFSET 0
#define SCREEN_BOTTOM_COMMON_TEXTDATA_FIELD_FPS_X_OFFSET 33
#define SCREEN_BOTTOM_COMMON_TEXTDATA_FIELD_RUNTIME_X_OFFSET 130
#define SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET 226

LGFXScreenDashboardResume::LGFXScreenDashboardResume(LovyanGFX *display, SourceData *sourceData) : LGFXScreen(display)
{
    if (sourceData != nullptr)
    {
        this->currentSourceData = sourceData;
        if (display != nullptr)
        {
            this->cpuLoadMeter = new LGFXMeter(this->parentDisplay, ET_GLOBAL_CPU_LOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 0, METER_GRAPH_BG_COLOR, "CPU LOAD", this->currentSourceData->getMinCPULoad(), this->currentSourceData->getMaxCPULoad());
            this->memoryLoadMeter = new LGFXMeter(this->parentDisplay, ET_USED_MEMORY, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 42, METER_GRAPH_BG_COLOR, "MEMORY", 0, this->currentSourceData->getTotalMemory());
            this->cpuTemperatureLoadMeter = new LGFXMeter(this->parentDisplay, ET_GLOBAL_CPU_TEMPERATURE, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 84, METER_GRAPH_BG_COLOR, "CPU TEMP", this->currentSourceData->getMinCPUTemperature(), this->currentSourceData->getMaxCPUTemperature());
            this->networkDownloadBandwithLoadMeter = new LGFXMeter(this->parentDisplay, ET_NETWORK_BANDWITH_DOWNLOAD_SPEED, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 126, METER_GRAPH_BG_COLOR, "WAN DOWNLOAD", 0, this->currentSourceData->getNetworkDownloadBandwidthLimit());
            this->networkUploadBandwithLoadMeter = new LGFXMeter(this->parentDisplay, ET_NETWORK_BANDWITH_UPLOAD_SPEED, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 168, METER_GRAPH_BG_COLOR, "WAN UPLOAD", 0, this->currentSourceData->getNetworkUploadBandwidthLimit());
            this->refresh(true);
        }
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
    if (this->currentSourceData != nullptr)
    {
        this->currentSourceData = nullptr;
    }
}

void LGFXScreenDashboardResume::refreshBottomCommonData(bool forceDrawAll)
{
    FPS::loop();
    if (forceDrawAll)
    {
        this->parentDisplay->setFont(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT);
        this->parentDisplay->setTextSize(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE);
        this->parentDisplay->setTextColor(SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR, SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR);
        this->parentDisplay->setCursor(SCREEN_BOTTOM_COMMON_TEXTDATA_X_OFFSET, SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET);
        // this->parentDisplay->print("FPS: ");
        this->parentDisplay->print("FPS:      - Runtime: ");
    }
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
    char str[sizeof(this->previousRuntimeStr)];
    Format::millisToHumanStr(millis(), str, sizeof(this->previousRuntimeStr));
    if (forceDrawAll || strcmp(str, this->previousRuntimeStr) != 0)
    {
        this->parentDisplay->setFont(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT);
        this->parentDisplay->setTextSize(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE);
        this->parentDisplay->setTextColor(SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR, SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR);
        this->parentDisplay->setCursor(SCREEN_BOTTOM_COMMON_TEXTDATA_FIELD_RUNTIME_X_OFFSET, SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET);
        this->parentDisplay->printf("%s    ", str);
        strncpy(this->previousRuntimeStr, str, sizeof(this->previousRuntimeStr));
    }
    // TODO: current uptime & other data
}

void LGFXScreenDashboardResume::refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        if (this->currentSourceData != nullptr)
        {
            uint64_t currentCPUMillis = this->currentSourceData->getCurrentCPULoadTimestamp();
            if (force || currentCPUMillis > this->lastCPUTimestamp)
            {
                this->cpuLoadMeter->refresh(this->currentSourceData->getCurrentCPULoad());
                this->lastCPUTimestamp = currentCPUMillis;
            }
            uint64_t currentMemoryMillis = this->currentSourceData->getCurrentUsedMemoryTimestamp();
            if (force || currentMemoryMillis > this->lastMemoryTimestamp)
            {
                this->memoryLoadMeter->refresh(this->currentSourceData->getUsedMemory());
                this->lastMemoryTimestamp = currentMemoryMillis;
            }
            uint64_t currentCPUTemperatureMillis = this->currentSourceData->getCurrentCPUTemperatureTimestamp();
            if (force || currentCPUTemperatureMillis > this->lastCPUTemperatureTimestamp)
            {
                this->cpuTemperatureLoadMeter->refresh(this->currentSourceData->getCurrentCPUTemperature());
                this->lastCPUTemperatureTimestamp = currentCPUTemperatureMillis;
            }
            uint64_t currentDownloadMillis = this->currentSourceData->getNetworkDownloadSpeedTimestamp();
            if (force || currentDownloadMillis > this->lastDownloadTimestamp)
            {
                this->networkDownloadBandwithLoadMeter->refresh(this->currentSourceData->getNetworkDownloadSpeed());
                this->lastDownloadTimestamp = currentDownloadMillis;
            }
            uint64_t currentUploadMillis = this->currentSourceData->getNetworkUploadSpeedTimestamp();
            if (force || currentUploadMillis > this->lastUploadTimestamp)
            {
                this->networkUploadBandwithLoadMeter->refresh(this->currentSourceData->getNetworkUploadSpeed());
                this->lastUploadTimestamp = currentUploadMillis;
            }
        }
        this->refreshBottomCommonData(force);
    }
}
