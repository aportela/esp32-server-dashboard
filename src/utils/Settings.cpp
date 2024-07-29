#include "Settings.hpp"

// WARNING: namespaces with length > 15 WILL NOT OPEN NVS CORRECTLY
#define NAMESPACE "esp32_srv_dashb"

#define KEY_WIFI_SSID "WIFI_SSID"
#define KEY_WIFI_PASSWORD "WIFI_PASSWORD"

Settings::Settings(void)
{
    this->preferences = new Preferences();
}

Settings::~Settings()
{
    delete this->preferences;
    this->preferences = nullptr;
}

void Settings::getValue(const char *key, char *value, size_t count)
{
    if (this->preferences->begin(NAMESPACE, false))
    {
        if (this->preferences->isKey(key))
        {
            this->preferences->getString(key, value, count);
        }
        else
        {
            if (count > 0)
            {
                value[0] = '\0';
            }
        }
        this->preferences->end();
    }
}

void Settings::setValue(const char *key, const char *value)
{
    if (this->preferences->begin(NAMESPACE, false))
    {
        if (this->preferences->putString(key, value))
        {
        }
        else
        {
        }
        this->preferences->end();
    }
}

void Settings::clear(void)
{
    if (this->preferences->begin(NAMESPACE, false))
    {
        this->preferences->clear();
        this->preferences->end();
    }
}

void Settings::getWIFISSID(char *ssid, size_t count)
{
    this->getValue(KEY_WIFI_SSID, ssid, count);
}

void Settings::setWIFISSID(const char *ssid)
{
    this->setValue(KEY_WIFI_SSID, ssid);
}

void Settings::getWIFIPassword(char *password, size_t count)
{
    this->getValue(KEY_WIFI_PASSWORD, password, count);
}

void Settings::setWIFIPassword(const char *password)
{
    this->setValue(KEY_WIFI_PASSWORD, password);
}
