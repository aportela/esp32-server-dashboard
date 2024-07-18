#include <LovyanGFX.hpp>
#include <stdint.h>
#include "../FPSDebug.hpp"
// #include "../../sources/ISource.hpp"

class LGFX : public lgfx::LGFX_Device
{
private:
    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::LGFX_Sprite *cpuLoadSprite = nullptr;
    lgfx::LGFX_Sprite *memorySprite = nullptr;
    lgfx::LGFX_Sprite *temperatureSprite = nullptr;
    lgfx::LGFX_Sprite *debugSprite = nullptr;

    uint32_t getTemperatureGradientFrom0To100(int8_t value);
    FPSDebug *fpsDebug;

    uint64_t startMillis;
    uint8_t oldCPULoad;

public:
    LGFX(uint8_t SDA, uint8_t SCL, uint8_t CS, uint8_t DC, uint8_t RST);
    ~LGFX();
    void initCPULoadMeter(uint8_t xOffset, uint8_t yOffset);
    void refreshCPULoadMeter(uint8_t xOffset, uint8_t yOffset, uint8_t load);
    void initMemoryMeter(uint8_t xOffset, uint8_t yOffset);
    void refreshMemoryMeter(uint8_t xOffset, uint8_t yOffset, uint64_t totalMemory, uint64_t usedMemory);
    void initNetworkMeter(uint8_t xOffset, uint8_t yOffset);
    void initTemperatureMeter(uint8_t xOffset, uint8_t yOffset);
    void refreshTemperatureMeter(uint8_t xOffset, uint8_t yOffset, uint8_t temperature);
    void refreshDebug(uint8_t xOffset, uint8_t yOffset);
};