#include "LGFXMeter.hpp"

#define AXIS_COLOR_ TFT_WHITE
#define GRAPH_LABEL__FONT_SIZE 1

#define MAIN_LABEL_COLOR TFT_WHITE
#define MAIN_LABEL_BACKGROUND TFT_BLACK

#define MAIN_LABEL_MIN_COLOR 0x001F

// by chatGPT
uint8_t LGFXMeter::map64WithRange0To00(uint64_t x, uint64_t in_min, uint64_t in_max)
{
    if (in_min == in_max)
    {
        return 0;
    }
    else if (x < in_min)
    {
        return (0);
    }
    else if (x > in_max)
    {
        return (100);
    }
    else
    {
        return 0 + (x - in_min) * (100 - 0) / (in_max - in_min);
    }
}

// TODO set min/max on constructor
LGFXMeter::LGFXMeter(LovyanGFX *display, MeterEntity entity, int32_t width, int32_t height, uint16_t xOffset, uint16_t yOffset, int32_t backgroundColor, char *label)
{
    this->parentDisplay = display;
    this->graphSprite = new lgfx::LGFX_Sprite(display);
    this->graphSprite->createSprite(width, height);
    this->graphSprite->fillSprite(backgroundColor);
    this->entity = entity;
    this->width = width;
    this->height = height;
    this->xOffset = xOffset;
    this->yOffset = yOffset;
    this->min = 0;
    this->max = 0;

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
    switch (this->entity)
    {
    case METER_ENTITY_CPU_LOAD:
        this->parentDisplay->print("000%");
        break;
    case METER_ENTITY_MEMORY:
        this->parentDisplay->print("000Gb  /  032Gb");
        break;
    case METER_ENTITY_CPU_TEMPERATURE:
        this->parentDisplay->print("000C");
        break;
    case METER_ENTITY_NETWORK_BANDWITH_DOWNLOAD:
        this->parentDisplay->print("000Mb  /  512Mb");
        break;
    case METER_ENTITY_NETWORK_BANDWITH_UPLOAD:
        this->parentDisplay->print("000Mb  /  512Mb");
        break;
    }

    this->previousMappedValue = 0;
    // WARNING: using TFT_BLACK will create some garbage because at this time, when Lovyann GFX write text with TFT_BLACK color & TFT_BLACK background do not overwrite old area correctly so I use the "first" (0) custom gradient color for a mapped (0-100) value
    this->previousGradientcolor = MAIN_LABEL_MIN_COLOR;
}

LGFXMeter::~LGFXMeter()
{
    delete this->graphSprite;
    this->graphSprite = nullptr;
}

void LGFXMeter::setMin(uint64_t value)
{
    this->min = value;
}

void LGFXMeter::setMax(uint64_t value)
{
    this->max = value;
}

uint32_t LGFXMeter::getGradientColorFrom0To100(uint8_t value)
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

void LGFXMeter::formatValueAsCPULoad(uint64_t value, char *label)
{
    sprintf(label, "%03d", value);
}

void LGFXMeter::formatValueAsMemory(uint64_t value, char *label)
{
    static const char *units[] = {"B  ", "Kb ", "Mb ", "Gb ", "Tb ", "Pb "};
    static const uint8_t numUnits = sizeof(units) / sizeof(units[0]) - 1;
    uint8_t currentUnitIndex = 0;
    double decValue = (double)value;
    while (decValue >= 1024.0 && currentUnitIndex < numUnits)
    {
        decValue /= 1024;
        currentUnitIndex++;
    }
    sprintf(label, "%03d", (int)decValue);
    strcat(label, units[currentUnitIndex]);
}

void LGFXMeter::refresh(uint64_t value)
{
    uint8_t mapped100 = 0;
    switch (this->entity)
    {
    case METER_ENTITY_CPU_LOAD:
        mapped100 = map(value, this->min, this->max, 0, 100);
        break;
    case METER_ENTITY_MEMORY:
        mapped100 = map64WithRange0To00(value, this->min, this->max);
        break;
    case METER_ENTITY_CPU_TEMPERATURE:
        mapped100 = map(value, 0, 100, 0, 100);
        break;
    case METER_ENTITY_NETWORK_BANDWITH_DOWNLOAD:
        mapped100 = map64WithRange0To00(value, this->min, this->max);
        break;
    case METER_ENTITY_NETWORK_BANDWITH_UPLOAD:
        mapped100 = map64WithRange0To00(value, this->min, this->max);
        break;
    }
    int32_t color = value > 0 ? (mapped100 != this->previousMappedValue) ? this->getGradientColorFrom0To100(mapped100) : this->previousGradientcolor : TFT_WHITE;
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
        char formattedValueLabel[16];
        switch (this->entity)
        {
        case METER_ENTITY_CPU_LOAD:
            this->formatValueAsCPULoad(value, formattedValueLabel);
            break;
        case METER_ENTITY_MEMORY:
            this->formatValueAsMemory(value, formattedValueLabel);
            break;
        case METER_ENTITY_CPU_TEMPERATURE:
            this->formatValueAsCPULoad(value, formattedValueLabel);
            break;
        case METER_ENTITY_NETWORK_BANDWITH_DOWNLOAD:
            this->formatValueAsMemory(value, formattedValueLabel);
            break;
        case METER_ENTITY_NETWORK_BANDWITH_UPLOAD:
            this->formatValueAsMemory(value, formattedValueLabel);
            break;
        }
        this->parentDisplay->print(formattedValueLabel);
        this->previousValue = value;
    }
}