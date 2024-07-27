#include "LGFXScreen.hpp"

LGFXScreen::LGFXScreen(LovyanGFX *display) : parentDisplay(display)
{
    if (display != nullptr)
    {
        this->parentDisplay->fillScreen(TFT_BLACK);
    }
}

LGFXScreen::~LGFXScreen()
{
    this->parentDisplay = nullptr;
}
