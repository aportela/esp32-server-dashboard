#include "LGFXScreen.hpp"

LGFXScreen::LGFXScreen(LovyanGFX *display)
{
    this->parentDisplay = display;
    this->parentDisplay->fillScreen(TFT_BLACK);
}

LGFXScreen::~LGFXScreen()
{
    this->parentDisplay = nullptr;
}
