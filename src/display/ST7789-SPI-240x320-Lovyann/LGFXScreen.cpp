#include "LGFXScreen.hpp"

LGFXScreen::LGFXScreen(LovyanGFX *display, FPSDebug *fpsDebug, SystemStatus *sysStats)
{
    this->parentDisplay = display;
    this->parentDisplay->fillScreen(TFT_BLACK);
    this->fpsDebug = fpsDebug;
    this->sysStats = sysStats;
}

LGFXScreen::~LGFXScreen()
{
    this->parentDisplay = nullptr;
    this->fpsDebug = nullptr;
    this->sysStats = nullptr;
}
