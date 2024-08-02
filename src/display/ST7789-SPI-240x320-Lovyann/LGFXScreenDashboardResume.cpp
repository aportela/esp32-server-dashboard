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
#define SCREEN_BOTTOM_COMMON_TEXTDATA_FIELD_UPTIME_X_OFFSET 123
#define SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET 226

LGFXScreenDashboardResume::LGFXScreenDashboardResume(LovyanGFX *display, SourceData *sourceData) : LGFXScreen(display)
{
    if (sourceData != nullptr)
    {
        this->currentSourceData = sourceData;
        if (display != nullptr)
        {
            this->dre = new LGFXScreenDashboardResumeEntityCPULoad(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 0, true);
            this->dre2 = new LGFXScreenDashboardResumeEntityUsedMemory(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 42, true);
            this->dre3 = new LGFXScreenDashboardResumeEntityCPUTemperature(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 84, true);
            this->dre4 = new LGFXScreenDashboardResumeEntityNetUsedBandWidth(NBT_DOWNLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 126, true);
            this->dre5 = new LGFXScreenDashboardResumeEntityNetUsedBandWidth(NBT_UPLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 168, true);
            this->refresh(true);
        }
    }
}

LGFXScreenDashboardResume::~LGFXScreenDashboardResume()
{
    if (this->dre != nullptr)
    {
        delete this->dre;
        this->dre = nullptr;
    }
    if (this->dre2 != nullptr)
    {
        delete this->dre2;
        this->dre2 = nullptr;
    }
    if (this->dre3 != nullptr)
    {
        delete this->dre3;
        this->dre3 = nullptr;
    }
    if (this->dre4 != nullptr)
    {
        delete this->dre4;
        this->dre4 = nullptr;
    }
    if (this->dre5 != nullptr)
    {
        delete this->dre5;
        this->dre5 = nullptr;
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
        Serial.println(this->parentDisplay->getCursorX());
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
            /*
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
            */
            this->dre->refresh(false);
            this->dre2->refresh(false);
            this->dre3->refresh(false);
            this->dre4->refresh(false);
            this->dre5->refresh(false);
        }
        this->refreshBottomCommonData(force);
    }
}
