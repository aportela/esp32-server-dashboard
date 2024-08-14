#include "LGFXScreenDashboardResume.hpp"
#include <inttypes.h>
#include "../SizesAndOffsets-320x240.hpp"
#include "../../utils/WifiManager.hpp"
#include "../../utils/FPS.hpp"
#include "../../utils/Format.hpp"

#define METER_GRAPH_WIDTH 185
#define METER_GRAPH_HEIGHT 34
#define METER_GRAPH_BG_COLOR TFT_BLACK

#define SCREEN_BOTTOM_COMMON_TEXTDATA_FONT &fonts::Font2
#define SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE 1
#define SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR TFT_WHITE
#define SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR TFT_BLACK
#define SCREEN_BOTTOM_COMMON_TEXTDATA_X_OFFSET 0
#define SCREEN_BOTTOM_COMMON_TEXTDATA_FIELD_FPS_X_OFFSET 33
#define SCREEN_BOTTOM_COMMON_TEXTDATA_FIELD_UPTIME_X_OFFSET 123
#define SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET 224

LGFXScreenDashboardResume::LGFXScreenDashboardResume(LovyanGFX *display, SourceData *sourceData) : LGFXScreen(display)
{
    if (sourceData != nullptr)
    {
        this->currentSourceData = sourceData;
        if (display != nullptr)
        {
            FPS::init();
            this->cpuLoadBlock = new LGFXScreenDashboardResumeEntityCPULoad(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 0);
            this->usedMemoryBlock = new LGFXScreenDashboardResumeEntityUsedMemory(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 1);
            this->cpuTemperatureBlock = new LGFXScreenDashboardResumeEntityCPUTemperature(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 2);
            this->networkDownloadBandwidthBlock = new LGFXScreenDashboardResumeEntityNetUsedBandWidth(NBT_DOWNLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 3);
            this->networkUploadBandwidthBlock = new LGFXScreenDashboardResumeEntityNetUsedBandWidth(NBT_UPLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 4);
            // this->networkDownloadBandwidthBlock = new LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NBT_DOWNLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 3);
            // this->networkUploadBandwidthBlock = new LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NBT_UPLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 4);
            this->refresh(true);
        }
    }
}

LGFXScreenDashboardResume::~LGFXScreenDashboardResume()
{
    if (this->cpuLoadBlock != nullptr)
    {
        delete this->cpuLoadBlock;
        this->cpuLoadBlock = nullptr;
    }
    if (this->usedMemoryBlock != nullptr)
    {
        delete this->usedMemoryBlock;
        this->usedMemoryBlock = nullptr;
    }
    if (this->cpuTemperatureBlock != nullptr)
    {
        delete this->cpuTemperatureBlock;
        this->cpuTemperatureBlock = nullptr;
    }
    if (this->networkDownloadBandwidthBlock != nullptr)
    {
        delete this->networkDownloadBandwidthBlock;
        this->networkDownloadBandwidthBlock = nullptr;
    }
    if (this->networkUploadBandwidthBlock != nullptr)
    {
        delete this->networkUploadBandwidthBlock;
        this->networkUploadBandwidthBlock = nullptr;
    }
    if (this->currentSourceData != nullptr)
    {
        this->currentSourceData = nullptr;
    }
}

bool LGFXScreenDashboardResume::refreshBottomCommonData(bool forceDrawAll)
{
    bool changed = forceDrawAll;
    FPS::loop(999);
    if (forceDrawAll)
    {
        this->parentDisplay->setFont(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT);
        this->parentDisplay->setTextSize(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE);
        this->parentDisplay->setTextColor(SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR, SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR);
        this->parentDisplay->setCursor(SCREEN_BOTTOM_COMMON_TEXTDATA_X_OFFSET, SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET);
        this->parentDisplay->print("FPS:      - Uptime: NO DATA");
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
        changed = true;
    }
    if (this->currentSourceData->getCurrentUptimeSeconds() > 0)
    {
        char str[sizeof(this->previousUptimeStr)];
        Format::millisToHumanStr(this->currentSourceData->getCurrentUptimeSeconds() * 1000, str, sizeof(this->previousUptimeStr));
        if (forceDrawAll || strcmp(str, this->previousUptimeStr) != 0)
        {
            this->parentDisplay->setFont(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT);
            this->parentDisplay->setTextSize(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE);
            this->parentDisplay->setTextColor(SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR, SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR);
            this->parentDisplay->setCursor(SCREEN_BOTTOM_COMMON_TEXTDATA_FIELD_UPTIME_X_OFFSET, SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET);
            this->parentDisplay->printf("%s    ", str);
            strncpy(this->previousUptimeStr, str, sizeof(this->previousUptimeStr));
            changed = true;
        }
    }
    return (changed);
}

bool LGFXScreenDashboardResume::refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        bool refreshed = false;
        if (this->currentSourceData != nullptr)
        {
            bool r1 = this->cpuLoadBlock->refresh(false);
            bool r2 = this->usedMemoryBlock->refresh(false);
            bool r3 = this->cpuTemperatureBlock->refresh(false);
            bool r4 = this->networkDownloadBandwidthBlock->refresh(false);
            bool r5 = this->networkUploadBandwidthBlock->refresh(false);
            refreshed = r1 || r2 || r3 || r4 || r5;
        }
        return (this->refreshBottomCommonData(force) || refreshed);
    }
    else
    {
        return (false);
    }
}
