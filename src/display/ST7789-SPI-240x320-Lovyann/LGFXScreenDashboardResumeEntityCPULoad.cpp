#include "LGFXScreenDashboardResumeEntityCPULoad.hpp"
#include <math.h>

LGFXScreenDashboardResumeEntityCPULoad::LGFXScreenDashboardResumeEntityCPULoad(LovyanGFX *display, SourceData *sourceData, uint16_t xOffset, uint16_t yOffset, bool enabled) : LGFXScreenDashboardResumeEntity(display, sourceData, xOffset, yOffset, "CPU LOAD", enabled)
{
    if (this->parentDisplay != nullptr)
    {
        // this->graph =
    }
}

LGFXScreenDashboardResumeEntityCPULoad::~LGFXScreenDashboardResumeEntityCPULoad()
{
    if (this->graph != nullptr)
    {
        // delete this->graph;
        // this->graph = nullptr;
    }
}

bool LGFXScreenDashboardResumeEntityCPULoad::refresh(bool force)
{
    uint64_t currentTimestamp = this->sourceData->getCurrentCPULoadTimestamp();
    if (this->sourceData->changedCPULoad(this->timestamp) || force)
    {
        float currentValue = this->sourceData->getCurrentCPULoad();
        this->timestamp = currentTimestamp;
        if (currentValue != this->value || force)
        {
            Serial.println(currentValue);
            std::snprintf(strValue, sizeof(strValue), "%03d.%1d%%", (int16_t)currentValue, (int16_t)((currentValue - (int16_t)currentValue) * 10));
            Serial.println(strValue);
            this->parentDisplay->setFont(LGFX_SCR_DRE_FONT);
            this->parentDisplay->setTextSize(LGFX_SCR_DRE_FONT_SIZE);
            this->parentDisplay->setTextColor(LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
            this->parentDisplay->setCursor(this->xOffset + 195 + 12, this->yOffset + 19);
            this->parentDisplay->print(strValue);
            return (true);
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}
