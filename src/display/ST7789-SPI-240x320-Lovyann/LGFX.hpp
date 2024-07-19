#include <LovyanGFX.hpp>
#include <stdint.h>
#include "../FPSDebug.hpp"
#include "../../sources/ISource.hpp"

class LGFX : public lgfx::LGFX_Device
{
private:
    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::LGFX_Sprite *cpuLoadSprite = nullptr;
    lgfx::LGFX_Sprite *memorySprite = nullptr;
    lgfx::LGFX_Sprite *cpuTemperatureSprite = nullptr;
    lgfx::LGFX_Sprite *networkDownloadSprite = nullptr;
    lgfx::LGFX_Sprite *networkUploadSprite = nullptr;
    lgfx::LGFX_Sprite *debugSprite = nullptr;

    FPSDebug *fpsDebug;

    uint64_t startMillis;
    ISource *source;

    uint32_t getTemperatureGradientFrom0To100(int8_t value);
    void initGraph(uint16_t xOffset, uint16_t yOffset, char *label);
    void refreshGraphSprite(lgfx::LGFX_Sprite *sprite, uint8_t valueMappedTo100, int32_t color, uint16_t xOffset, uint16_t yOffset);

public:
    LGFX(uint8_t PIN_SDA, uint8_t PIN_SCL, uint8_t PIN_CS, uint8_t PIN_DC, uint8_t PIN_RST, uint16_t width, uint16_t height, uint8_t rotation);
    ~LGFX();
    void setSource(ISource *src);
    void initCPULoadMeter(uint16_t xOffset, uint16_t yOffset);
    void refreshCPULoadMeter(uint16_t xOffset, uint16_t yOffset);
    void initMemoryMeter(uint16_t xOffset, uint16_t yOffset);
    void refreshMemoryMeter(uint16_t xOffset, uint16_t yOffset);
    void initCPUTemperatureMeter(uint16_t xOffset, uint16_t yOffset);
    void refreshCPUTemperatureMeter(uint16_t xOffset, uint16_t yOffset);
    void initNetworkDownloadBandwithMeter(uint16_t xOffset, uint16_t yOffset);
    void refreshNetworkDownloadBandwithMeter(uint16_t xOffset, uint16_t yOffset);
    void initNetworkUploadBandwithMeter(uint16_t xOffset, uint16_t yOffset);
    void refreshNetworkUploadBandwithMeter(uint16_t xOffset, uint16_t yOffset);
    void refreshDebug(uint16_t xOffset, uint16_t yOffset);
};