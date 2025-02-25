#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_H

#include <LovyanGFX.hpp>
#include "../../sources/SourceData.hpp"

#define LGFX_SCR_DRE_SMALL_FONT &fonts::Font0
#define LGFX_SCR_DRE_SMALL_FONT_SIZE 1

#define LGFX_SCR_DRE_FONT &fonts::Font2
#define LGFX_SCR_DRE_FONT_SIZE 1
#define LGFX_SCR_DRE_FONT_COLOR TFT_WHITE
#define LGFX_SCR_DRE_FONT_BG_COLOR TFT_BLACK

#define LGFX_SCR_DRE_TITLE_XOFFSET

class LGFXScreenDashboardResumeEntity
{
private:
    void InitGraph(void);
    void PrintTitle(const char *title);

protected:
    LovyanGFX *parentDisplay = nullptr;
    lgfx::LGFX_Sprite *graphSprite = nullptr;
    SourceData *sourceData = nullptr;
    uint16_t previousGradientcolor;
    uint8_t previousMappedValue;
    uint16_t width;
    uint16_t height;
    uint16_t xOffset = 0;
    uint16_t yOffset = 0;

    void PrintLimits(const char *low, const char *high);
    void ClearSprite(void);
    void DumpSprite(void);
    void RefreshSprite(uint8_t mapped100Value, int32_t color, bool dump = true);
    void RefreshStrValue(const char *strValue, int32_t color = LGFX_SCR_DRE_FONT_COLOR, int32_t background = LGFX_SCR_DRE_FONT_BG_COLOR);
    uint8_t MapFloatValueFrom0To100(float value, float minValue, float maxValue);
    uint8_t MapUint64ValueFrom0To100(uint64_t value, uint64_t minValue, uint64_t maxValue);
    uint16_t GetGradientColorFrom0To100(uint8_t value);

public:
    LGFXScreenDashboardResumeEntity(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, const char *title);
    ~LGFXScreenDashboardResumeEntity();
    virtual bool Refresh(bool force = false) = 0;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_H
