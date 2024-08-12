#include "LGFXScreenDashboardResumeEntity.hpp"

#define GRAPH_BG_COLOR TFT_BLACK
#define GRAPH_AXIS_COLOR TFT_WHITE

#define INIT_GRADIENT_COLOR 0x001F

LGFXScreenDashboardResumeEntity::LGFXScreenDashboardResumeEntity(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, const char *title) : width(width), height(height), xOffset(xOffset), yOffset(yOffset)
{
    if (display != nullptr)
    {
        this->parentDisplay = display;
        this->previousMappedValue = 0;
        this->previousGradientcolor = INIT_GRADIENT_COLOR;
        this->initGraph();
        this->printTitle(title);
    }
    if (sourceData != nullptr)
    {
        this->sourceData = sourceData;
    }
}

LGFXScreenDashboardResumeEntity::~LGFXScreenDashboardResumeEntity()
{
    if (this->graphSprite != nullptr)
    {
        delete this->graphSprite;
        this->graphSprite = nullptr;
    }
    if (this->sourceData != nullptr)
    {
        this->sourceData = nullptr;
    }
    if (this->parentDisplay != nullptr)
    {
        this->parentDisplay = nullptr;
    }
}

void LGFXScreenDashboardResumeEntity::initGraph(void)
{
    this->graphSprite = new lgfx::LGFX_Sprite(this->parentDisplay);
    this->graphSprite->createSprite(width, height);
    this->graphSprite->fillSprite(GRAPH_BG_COLOR);

    this->parentDisplay->drawFastVLine(this->xOffset, this->yOffset, (this->height + 4), GRAPH_AXIS_COLOR);
    this->parentDisplay->drawFastHLine(this->xOffset, this->yOffset + (this->height + 4), (this->width + 4), GRAPH_AXIS_COLOR);

    this->parentDisplay->drawFastVLine(this->xOffset + (this->width + 4) - 1, this->yOffset, (this->height + 4), GRAPH_AXIS_COLOR);
    this->parentDisplay->drawFastHLine(this->xOffset, this->yOffset, (this->width + 4), GRAPH_AXIS_COLOR);
}

void LGFXScreenDashboardResumeEntity::printTitle(const char *title)
{
    this->parentDisplay->setFont(LGFX_SCR_DRE_FONT);
    this->parentDisplay->setTextSize(LGFX_SCR_DRE_FONT_SIZE);
    this->parentDisplay->setTextColor(LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
    this->parentDisplay->setCursor(this->xOffset + this->width + 12, this->yOffset);
    this->parentDisplay->print(title);
}

void LGFXScreenDashboardResumeEntity::clearSprite(void)
{
    this->graphSprite->fillSprite(GRAPH_BG_COLOR);
}

void LGFXScreenDashboardResumeEntity::dumpSprite(void)
{
    this->graphSprite->pushSprite(this->xOffset + 2, this->yOffset + 2);
}

void LGFXScreenDashboardResumeEntity::refreshSprite(uint8_t mapped100Value, int32_t color, bool dump)
{
    uint8_t mappedGraphValue = map(mapped100Value, 0, 100, 0, this->height);
    // create graph animation moving sprite to left 1 pixel
    this->graphSprite->scroll(-1, 0);
    // draw new value (on right)
    this->graphSprite->drawFastVLine(this->width - 1, this->height - mappedGraphValue + 1, mappedGraphValue, color);
    // dump sprite to screen
    if (dump)
    {
        this->graphSprite->pushSprite(this->xOffset + 2, this->yOffset + 2);
    }
}

void LGFXScreenDashboardResumeEntity::refreshStrValue(const char *strValue, int32_t color, int32_t background)
{
    this->parentDisplay->setFont(LGFX_SCR_DRE_FONT);
    this->parentDisplay->setTextSize(LGFX_SCR_DRE_FONT_SIZE);
    this->parentDisplay->setTextColor(color, background);
    this->parentDisplay->setCursor(this->xOffset + 195 + 12, this->yOffset + 19);
    this->parentDisplay->print(strValue);
}

uint8_t LGFXScreenDashboardResumeEntity::mapFloatValueFrom0To100(float value, float minValue, float maxValue)
{
    return (map((int32_t)value, (int32_t)minValue, (int32_t)maxValue, 0, 100));
}

uint8_t LGFXScreenDashboardResumeEntity::mapUint64ValueFrom0To100(uint64_t value, uint64_t minValue, uint64_t maxValue)
{
    if (minValue == maxValue)
    {
        return (0);
    }
    else if (value < minValue)
    {
        return (0);
    }
    else if (value > maxValue)
    {
        return (100);
    }
    else
    {
        return (0 + (value - minValue) * (100 - 0) / (maxValue - minValue));
    }
}

uint16_t LGFXScreenDashboardResumeEntity::getGradientColorFrom0To100(uint8_t value)
{
    value = constrain(value, 0, 100);
    // blue -> green
    if (value <= 33)
    {
        int blue = 255;
        int green = map(value, 0, 33, 0, 255);
        return this->parentDisplay->color565(0, green, blue);
    }
    // green -> yellow
    else if (value <= 66)
    {
        int green = 255;
        int red = map(value, 34, 66, 0, 255);
        return this->parentDisplay->color565(red, green, 0);
    }
    // yellow -> red
    else
    {
        int red = 255;
        int green = map(value, 67, 100, 255, 0);
        return this->parentDisplay->color565(red, green, 0);
    }
}
