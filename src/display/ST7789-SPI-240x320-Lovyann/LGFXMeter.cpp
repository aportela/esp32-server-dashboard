#include "LGFXMeter.hpp"

#define AXIS_COLOR_ TFT_WHITE
#define GRAPH_LABEL__FONT_SIZE 1

#define MAIN_LABEL_COLOR TFT_WHITE
#define MAIN_LABEL_BACKGROUND TFT_BLACK

LGFXMeter::LGFXMeter(LovyanGFX *display, int32_t width, int32_t height, uint16_t xOffset, uint16_t yOffset, int32_t backgroundColor, char *label)
{
    this->parentDisplay = display;
    this->graphSprite = new lgfx::LGFX_Sprite(display);
    this->graphSprite->createSprite(width, height);
    this->graphSprite->fillSprite(backgroundColor);
    this->width = width;
    this->height = height;
    this->xOffset = xOffset;
    this->yOffset = yOffset;

    this->parentDisplay->drawFastVLine(xOffset, yOffset, (height + 4), AXIS_COLOR_);
    this->parentDisplay->drawFastHLine(xOffset, yOffset + (height + 4), (width + 4), AXIS_COLOR_);

    this->parentDisplay->drawFastVLine(xOffset + (width + 4) - 1, yOffset, (height + 4), AXIS_COLOR_);
    this->parentDisplay->drawFastHLine(xOffset, yOffset, (width + 4), AXIS_COLOR_);

    this->valueLabelXOffset = xOffset + width + 12;
    this->valueLabelYOffset = yOffset + 19;
    this->parentDisplay->setCursor(this->valueLabelXOffset, yOffset);
    this->parentDisplay->setTextSize(GRAPH_LABEL__FONT_SIZE);
    this->parentDisplay->setTextColor(MAIN_LABEL_COLOR, MAIN_LABEL_BACKGROUND);
    this->parentDisplay->print(label);

    this->parentDisplay->setCursor(this->valueLabelXOffset, this->valueLabelYOffset);
    this->parentDisplay->setTextSize(GRAPH_LABEL__FONT_SIZE);
    this->parentDisplay->setTextColor(MAIN_LABEL_COLOR, MAIN_LABEL_BACKGROUND);
    this->parentDisplay->print("000%");

    this->previousMappedValue = 0;
    this->previousGradientcolor = 0;
}

LGFXMeter::~LGFXMeter()
{
    delete this->graphSprite;
    this->graphSprite = nullptr;
}

uint32_t LGFXMeter::getGradientColorFrom0To100(int8_t value)
{
    // NOT REQUIRED
    // value = constrain(value, 0, 100);

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

void LGFXMeter::refresh(uint64_t value)
{
    uint8_t mapped100 = map(value, 0, 100, 0, 100);
    int32_t color = (mapped100 != this->previousMappedValue) ? this->getGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
    this->previousMappedValue = mapped100;
    this->previousGradientcolor = color;
    uint8_t mappedGraphValue = map(mapped100, 0, 100, 0, this->height);
    // create graph animation moving sprite to left 1 pixel
    this->graphSprite->scroll(-1, 0);
    // draw new value (on right)
    this->graphSprite->drawFastVLine(this->width - 1, this->height - mappedGraphValue + 1, mappedGraphValue, color);
    // dump sprite to screen
    this->graphSprite->pushSprite(this->xOffset + 2, this->yOffset + 2);

    // label value update
    if (value != this->previousValue)
    {
        this->parentDisplay->setCursor(this->valueLabelXOffset, this->valueLabelYOffset);
        this->parentDisplay->setTextSize(GRAPH_LABEL__FONT_SIZE);
        this->parentDisplay->setTextColor(color, MAIN_LABEL_BACKGROUND);
        sprintf(this->formattedValueLabel, "%03d", value);
        this->parentDisplay->print(this->formattedValueLabel);
        this->previousValue = value;
    }
}