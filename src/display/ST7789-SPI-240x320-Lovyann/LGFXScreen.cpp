#include "LGFXScreen.hpp"

LGFXScreen::LGFXScreen(LovyanGFX *display, FPSDebug *fpsDebug)
{
    this->parentDisplay = display;
    this->fpsDebug = fpsDebug;
    this->parentDisplay->fillScreen(TFT_BLACK);
}

LGFXScreen::~LGFXScreen()
{
    this->parentDisplay = nullptr;
    this->fpsDebug = nullptr;
}
