#include "Settings.hpp"

Settings::Settings(void)
{
    this->preferences = new Preferences();
    this->preferences->begin("esp32-server-dashboard", false);
}

Settings::~Settings()
{
    this->preferences->end();
    delete this->preferences;
    this->preferences = nullptr;
}

void Settings::clear(void)
{
    this->preferences->clear();
}
