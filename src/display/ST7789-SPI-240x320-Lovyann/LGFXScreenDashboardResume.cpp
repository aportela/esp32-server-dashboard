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

LGFXScreenDashboardResume::LGFXScreenDashboardResume(LovyanGFX *display, Source *source) : LGFXScreen(display)
{
    if (display != nullptr)
    {
        this->cpuLoadMeter = new LGFXMeter(this->parentDisplay, ET_GLOBAL_CPU_LOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 0, METER_GRAPH_BG_COLOR, "CPU LOAD", 0, 100);
        this->memoryLoadMeter = new LGFXMeter(this->parentDisplay, ET_USED_MEMORY, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 42, METER_GRAPH_BG_COLOR, "MEMORY", 0, Settings::getTotalMemoryBytes());
        this->cpuTemperatureLoadMeter = new LGFXMeter(this->parentDisplay, ET_GLOBAL_CPU_TEMPERATURE, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 84, METER_GRAPH_BG_COLOR, "CPU TEMP", 0, 100);
        this->networkDownloadBandwithLoadMeter = new LGFXMeter(this->parentDisplay, ET_NETWORK_BANDWITH_DOWNLOAD_SPEED, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 126, METER_GRAPH_BG_COLOR, "WAN DOWNLOAD", 0, Settings::getMaxDownloadBandwidthBytes());
        this->networkUploadBandwithLoadMeter = new LGFXMeter(this->parentDisplay, ET_NETWORK_BANDWITH_UPLOAD_SPEED, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 168, METER_GRAPH_BG_COLOR, "WAN UPLOAD", 0, Settings::getMaxUploadBandwidthBytes());
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

uint64_t lastCPUTimestamp = 0;
uint64_t lastMemoryTimestamp = 0;
uint64_t lastCPUTemperatureTimestamp = 0;
uint64_t lastDownloadTimestamp = 0;
uint64_t lastUploadTimestamp = 0;

void LGFXScreenDashboardResume::refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        if (this->currentSource != nullptr)
        {
            // uint64_t currentCPUMillis = this->currentSource->getCurrentTimestamp(ET_GLOBAL_CPU_LOAD);
            /*
            uint64_t currentCPUMillis = this->currentSource->getCurrentTimestamp(ET_GLOBAL_CPU_LOAD);
            if (force || currentCPUMillis > lastCPUTimestamp)
            {
                // this->cpuLoadMeter->refresh(this->currentSource->getCurrentGlobalCPULoad());
                lastCPUTimestamp = currentCPUMillis;
            }
            uint64_t currentMemoryMillis = this->currentSource->getCurrentTimestamp(ET_USED_MEMORY);
            if (force || currentMemoryMillis > lastMemoryTimestamp)
            {
                // this->memoryLoadMeter->refresh(this->currentSource->getUsedMemory());
                lastMemoryTimestamp = currentMemoryMillis;
            }
            uint64_t currentCPUTemperatureMillis = this->currentSource->getCurrentTimestamp(ET_GLOBAL_CPU_TEMPERATURE);
            if (force || currentCPUTemperatureMillis > lastCPUTemperatureTimestamp)
            {
                // this->cpuTemperatureLoadMeter->refresh(this->currentSource->getCurrentGlobalCPUTemperature());
                lastCPUTemperatureTimestamp = currentCPUTemperatureMillis;
            }
            uint64_t currentDownloadMillis = this->currentSource->getCurrentTimestamp(ET_NETWORK_BANDWITH_DOWNLOAD_SPEED);
            if (force || currentDownloadMillis > lastDownloadTimestamp)
            {
                // this->networkDownloadBandwithLoadMeter->refresh(this->currentSource->getCurrentNetworkDownloadUsedBandwidth());
                lastDownloadTimestamp = currentDownloadMillis;
            }
            uint64_t currentUploadMillis = this->currentSource->getCurrentTimestamp(ET_NETWORK_BANDWITH_UPLOAD_SPEED);
            if (force || currentUploadMillis > lastUploadTimestamp)
            {
                // this->networkUploadBandwithLoadMeter->refresh(this->currentSource->getCurrentNetworkUploadUsedBandwidth());
                lastUploadTimestamp = currentUploadMillis;
            }
            */
        }
        this->refreshBottomCommonData(force);
    }
}
