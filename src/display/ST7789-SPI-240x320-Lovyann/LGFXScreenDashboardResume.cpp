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

LGFXScreenDashboardResume::LGFXScreenDashboardResume(LovyanGFX *display, SourceData *sourceData, const uint8_t dashboardIndex, const DASHBOARD_ITEM_TYPE items[DASHBOARD_ITEM_COUNT]) : LGFXScreen(display)
{
    if (sourceData != nullptr)
    {
        this->currentSourceData = sourceData;
        this->dashboardIndex = dashboardIndex;
        if (display != nullptr)
        {
#ifdef DEBUG_FPS
            FPS::Init();
#endif // DEBUG_FPS

            if (
                items[0] == DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_DOWNLOAD_BANDWIDTH || items[0] == DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_UPLOAD_BANDWIDTH ||
                items[1] == DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_DOWNLOAD_BANDWIDTH || items[1] == DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_UPLOAD_BANDWIDTH ||
                items[2] == DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_DOWNLOAD_BANDWIDTH || items[2] == DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_UPLOAD_BANDWIDTH ||
                items[3] == DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_DOWNLOAD_BANDWIDTH || items[3] == DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_UPLOAD_BANDWIDTH ||
                items[4] == DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_DOWNLOAD_BANDWIDTH || items[4] == DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_UPLOAD_BANDWIDTH)
            {
                SourceDataQueueNetworkingLimitsValue networkingLimits;
                sourceData->SetNetworkLimits(networkingLimits);
                // TODO: WARNING DO NOT WORK WITH FIXED & NOT FIXED
            }
            switch (items[0])
            {
            case DASHBOARD_ITEM_TYPE_CPU_LOAD:
            default:
                this->block1 = new LGFXScreenDashboardResumeEntityCPU(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 0, DASHBOARD_ITEM_TYPE_CPU_LOAD);
                break;
            }
            switch (items[1])
            {
            case DASHBOARD_ITEM_TYPE_MEM_USED:
            default:
                this->block2 = new LGFXScreenDashboardResumeEntityMemory(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 1, DASHBOARD_ITEM_TYPE_MEM_USED);
                break;
            }
            switch (items[2])
            {
            case DASHBOARD_ITEM_TYPE_CPU_TEMPERATURE:
            default:
                this->block3 = new LGFXScreenDashboardResumeEntityCPUTemperature(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 2);
                break;
            }
            /*
            switch (items[3])
            {
            case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_DOWNLOAD_BANDWIDTH:
                this->block4 = new LGFXScreenDashboardResumeEntityNetUsedBandWidth(NET_BANDWIDTH_TYPE_DOWNLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 3);
                break;
            case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_DOWNLOAD_DYNAMIC_BANDWIDTH:
                this->block4 = new LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NET_BANDWIDTH_TYPE_DOWNLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 3);
                break;
            case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_UPLOAD_BANDWIDTH:
                this->block4 = new LGFXScreenDashboardResumeEntityNetUsedBandWidth(NET_BANDWIDTH_TYPE_UPLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 3);
                break;
            case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_UPLOAD_DYNAMIC_BANDWIDTH:
                this->block4 = new LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NET_BANDWIDTH_TYPE_UPLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 3);
                break;
            }
            switch (items[4])
            {
            case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_DOWNLOAD_BANDWIDTH:
                this->block5 = new LGFXScreenDashboardResumeEntityNetUsedBandWidth(NET_BANDWIDTH_TYPE_DOWNLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 4);
                break;
            case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_DOWNLOAD_DYNAMIC_BANDWIDTH:
                this->block5 = new LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NET_BANDWIDTH_TYPE_DOWNLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 4);
                break;
            case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_UPLOAD_BANDWIDTH:
                this->block5 = new LGFXScreenDashboardResumeEntityNetUsedBandWidth(NET_BANDWIDTH_TYPE_UPLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 4);
                break;
            case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_UPLOAD_DYNAMIC_BANDWIDTH:
                this->block5 = new LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NET_BANDWIDTH_TYPE_UPLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 4);
                break;
            }
            */
            sourceData->GetHostname(this->hostname, sizeof(this->hostname));
            this->Refresh(true);
        }
    }
}

LGFXScreenDashboardResume::~LGFXScreenDashboardResume()
{
    if (this->block1 != nullptr)
    {
        delete this->block1;
        this->block1 = nullptr;
    }
    if (this->block2 != nullptr)
    {
        delete this->block2;
        this->block2 = nullptr;
    }
    if (this->block3 != nullptr)
    {
        delete this->block3;
        this->block3 = nullptr;
    }
    if (this->block4 != nullptr)
    {
        delete this->block4;
        this->block4 = nullptr;
    }
    if (this->block5 != nullptr)
    {
        delete this->block5;
        this->block5 = nullptr;
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
        this->parentDisplay->printf("[%d] ", this->dashboardIndex);
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
            bool r1 = this->block1->Refresh(false);
            bool r2 = this->block2->Refresh(false);
            bool r3 = this->block3->Refresh(false);
            //  bool r4 = this->block4->Refresh(false);
            //  bool r5 = this->block5->Refresh(false);
            refreshed = r1 || r2 || r3; // || r4 || r5;
        }
        return (this->RefreshBottomCommonData(force) || refreshed);
    }
    else
    {
        return (false);
    }
}
