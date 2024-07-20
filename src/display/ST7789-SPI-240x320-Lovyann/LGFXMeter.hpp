#ifndef ESP32_SERVER_DASHBOARD_LGFX_METER_H
#define ESP32_SERVER_DASHBOARD_LGFX_METER_H

#include <stdint.h>
#include <LovyanGFX.hpp>
#include "../../sources/MeterEntity.hpp"

class LGFXMeter
{
private:
    LovyanGFX *parentDisplay = nullptr;
    lgfx::LGFX_Sprite *graphSprite = nullptr;
    MeterEntity entity;
    int32_t width;
    int32_t height;
    uint16_t xOffset;
    uint16_t yOffset;

    uint16_t valueLabelXOffset;
    uint16_t valueLabelYOffset;

    // this is for preventing recalc data if no changes required
    uint64_t previousMappedValue;
    int32_t previousGradientcolor;
    uint64_t previousValue;

    uint8_t map64WithRange0To00(uint64_t x, uint64_t in_min, uint64_t in_max);
    void formatValueAsCPULoad(uint64_t value, char *label);
    void formatValueAsMemory(uint64_t value, char *label);

public:
    LGFXMeter(LovyanGFX *display, MeterEntity entity, int32_t width, int32_t height, uint16_t xOffset, uint16_t yOffset, int32_t backgroundColor, char *label);
    ~LGFXMeter();

    uint32_t getGradientColorFrom0To100(int8_t value);
    void refresh(uint64_t value);
};

#endif
