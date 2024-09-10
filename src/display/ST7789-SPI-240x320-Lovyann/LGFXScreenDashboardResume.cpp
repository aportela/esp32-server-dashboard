#include "LGFXScreenDashboardResume.hpp"
#include <inttypes.h>
#include <cstring>
#include "../SizesAndOffsets-320x240.hpp"
#include "../../utils/WiFiManager.hpp"
#ifdef DEBUG_FPS
#include "../../utils/FPS.hpp"
#endif // DEBUG_FPS
#include "../../utils/Format.hpp"

#define METER_GRAPH_WIDTH 185
#define METER_GRAPH_HEIGHT 34
#define METER_GRAPH_BG_COLOR TFT_BLACK

#define SCREEN_BOTTOM_COMMON_TEXTDATA_FONT &fonts::Font2
#define SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE 1
#define SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR TFT_WHITE
#define SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR TFT_BLACK
#define SCREEN_BOTTOM_COMMON_TEXTDATA_X_OFFSET 0
#define SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET 224

// #define DEBUG_FPS // If defined, this will replace bottom hostname info with "FPS" counter (used only to "bench" & DEBUG_FPS optimizations)

LGFXScreenDashboardResume::LGFXScreenDashboardResume(LovyanGFX *display, SourceData *sourceData) : LGFXScreen(display)
{
    if (sourceData != nullptr)
    {
        this->currentSourceData = sourceData;
        if (display != nullptr)
        {
#ifdef DEBUG_FPS
            FPS::Init();
#endif // DEBUG_FPS
            this->cpuLoadBlock = new LGFXScreenDashboardResumeEntityCPULoad(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 0, CPU_USAGE_TYPE_LOAD);
            this->usedMemoryBlock = new LGFXScreenDashboardResumeEntityUsedMemory(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 1, MEMORY_USAGE_TYPE_USED);
            this->cpuTemperatureBlock = new LGFXScreenDashboardResumeEntityCPUTemperature(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 2);
            if (sourceData->HasFixedNetworkingLimits())
            {
                this->networkDownloadBandwidthBlock = new LGFXScreenDashboardResumeEntityNetUsedBandWidth(NET_BANDWIDTH_TYPE_DOWNLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 3);
                this->networkUploadBandwidthBlock = new LGFXScreenDashboardResumeEntityNetUsedBandWidth(NET_BANDWIDTH_TYPE_UPLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 4);
            }
            else
            {
                SourceDataQueueNetworkingLimitsValue networkingLimits;
                sourceData->SetNetworkLimits(networkingLimits);
                this->networkDownloadBandwidthBlock = new LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NET_BANDWIDTH_TYPE_DOWNLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 3);
                this->networkUploadBandwidthBlock = new LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NET_BANDWIDTH_TYPE_UPLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 4);
            }
            sourceData->GetHostname(this->hostname, sizeof(this->hostname));
            this->Refresh(true);
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
    this->currentSourceData = nullptr;
}

bool LGFXScreenDashboardResume::RefreshBottomCommonData(bool forceDrawAll)
{
    bool changed = forceDrawAll;
    if (forceDrawAll)
    {
        this->parentDisplay->setFont(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT);
        this->parentDisplay->setTextSize(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE);
        this->parentDisplay->setTextColor(SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR, SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR);
        this->parentDisplay->setCursor(SCREEN_BOTTOM_COMMON_TEXTDATA_X_OFFSET, SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET);
#ifdef DEBUG_FPS
        this->parentDisplay->print("FPS: ");
        this->FPSValueXOffset = this->parentDisplay->getCursorX();
        this->parentDisplay->print("000 - Uptime: ");
        this->uptimeValueXOffset = this->parentDisplay->getCursorX();
        this->parentDisplay->print("NO DATA");
#else
        this->parentDisplay->print("Host: ");
        this->FPSValueXOffset = this->parentDisplay->getCursorX();
        this->parentDisplay->printf("%s - Uptime: ", strlen(this->hostname) > 0 ? this->hostname : "[NOT SET]");
        this->uptimeValueXOffset = this->parentDisplay->getCursorX();
        this->parentDisplay->print("NO DATA");
#endif
    }
#ifdef DEBUG_FPS
    uint16_t currentFPS = FPS::GetFPS();
    if (forceDrawAll || currentFPS != this->previousFPS)
    {
        this->parentDisplay->setFont(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT);
        this->parentDisplay->setTextSize(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE);
        this->parentDisplay->setTextColor(SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR, SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR);
        this->parentDisplay->setCursor(this->FPSValueXOffset, SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET);
        this->parentDisplay->printf("%03u", currentFPS);
        this->previousFPS = currentFPS;
        changed = true;
    }
#else
#endif // DEBUG_FPS

    SourceDataQueueUptimeValue data;
    this->currentSourceData->GetCurrentUptime(data);
    if (data.seconds > 0)
    {
        char str[sizeof(this->previousUptimeStr)];
        Format::ParseMillisToHumanString(data.seconds * 1000, str, sizeof(this->previousUptimeStr));
        if (forceDrawAll || strcmp(str, this->previousUptimeStr) != 0)
        {
            this->parentDisplay->setFont(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT);
            this->parentDisplay->setTextSize(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE);
            this->parentDisplay->setTextColor(SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR, SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR);
            this->parentDisplay->setCursor(uptimeValueXOffset, SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET);
            this->parentDisplay->printf("%s    ", str);
            strncpy(this->previousUptimeStr, str, sizeof(this->previousUptimeStr));
            changed = true;
        }
    }
    return (changed);
}

bool LGFXScreenDashboardResume::Refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        bool refreshed = false;
        if (this->currentSourceData != nullptr)
        {
            bool r1 = this->cpuLoadBlock->Refresh(false);
            bool r2 = this->usedMemoryBlock->Refresh(false);
            bool r3 = this->cpuTemperatureBlock->Refresh(false);
            bool r4 = this->networkDownloadBandwidthBlock->Refresh(false);
            bool r5 = this->networkUploadBandwidthBlock->Refresh(false);
            refreshed = r1 || r2 || r3 || r4 || r5;
        }
        return (this->RefreshBottomCommonData(force) || refreshed);
    }
    else
    {
        return (false);
    }
}
