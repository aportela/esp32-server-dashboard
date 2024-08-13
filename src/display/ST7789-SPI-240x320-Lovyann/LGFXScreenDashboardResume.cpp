#include "LGFXScreenDashboardResume.hpp"
#include <inttypes.h>
#include "../SizesAndOffsets-320x240.hpp"
#include "../../utils/WifiManager.hpp"
#include "../../utils/FPS.hpp"
#include "../../utils/Format.hpp"

#define METER_GRAPH_WIDTH 170
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
            // this->networkDownloadBandwidthBlock = new LGFXScreenDashboardResumeEntityNetUsedBandWidth(NBT_DOWNLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 126);
            // this->networkUploadBandwidthBlock = new LGFXScreenDashboardResumeEntityNetUsedBandWidth(NBT_UPLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 168);
            this->networkDownloadBandwidthBlock = new LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NBT_DOWNLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 3);
            this->networkUploadBandwidthBlock = new LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NBT_UPLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 4);
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

void LGFXScreenDashboardResume::refreshBottomCommonData(bool forceDrawAll)
{
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
        }
    }
}

void LGFXScreenDashboardResume::refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        if (this->currentSourceData != nullptr)
        {
            this->cpuLoadBlock->refresh(false);
            this->usedMemoryBlock->refresh(false);
            this->cpuTemperatureBlock->refresh(false);
            this->networkDownloadBandwidthBlock->refresh(false);
            this->networkUploadBandwidthBlock->refresh(false);
        }
        this->refreshBottomCommonData(force);
    }
}
