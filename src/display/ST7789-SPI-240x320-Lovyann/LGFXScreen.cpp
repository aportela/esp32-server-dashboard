#include "LGFXScreen.hpp"

LGFXScreen::LGFXScreen(LovyanGFX *display, FPSDebug *fpsDebug)
{
    this->parentDisplay = display;
    this->parentDisplay->fillScreen(TFT_BLACK);
    this->fpsDebug = fpsDebug;
}

LGFXScreen::~LGFXScreen()
{
    this->parentDisplay = nullptr;
    this->fpsDebug = nullptr;
}
