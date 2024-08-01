#include "LGFXScreenDashboardResumeEntity.hpp"

LGFXScreenDashboardResumeEntity::LGFXScreenDashboardResumeEntity(LovyanGFX *display, SourceData *sourceData, uint16_t xOffset, uint16_t yOffset, const char *title, bool enabled) : xOffset(xOffset), yOffset(yOffset), enabled(enabled)
{
    if (display != nullptr)
    {
        this->parentDisplay = display;
        this->parentDisplay->setFont(LGFX_SCR_DRE_FONT);
        this->parentDisplay->setTextSize(LGFX_SCR_DRE_FONT_SIZE);
        this->parentDisplay->setTextColor(LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
        this->parentDisplay->setCursor(this->xOffset + 195 + 12, this->yOffset);
        this->parentDisplay->print(title);
    }
    if (sourceData != nullptr)
    {
        this->sourceData = sourceData;
    }
}

LGFXScreenDashboardResumeEntity::~LGFXScreenDashboardResumeEntity()
{
    if (this->parentDisplay != nullptr)
    {
        this->parentDisplay = nullptr;
    }
    if (this->sourceData != nullptr)
    {
        this->sourceData = nullptr;
    }
}
