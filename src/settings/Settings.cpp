#include "Settings.hpp"
#include <nvs_flash.h>
#include "Arduino.h"

Settings::Settings(void)
{
    Serial.println("Starting settings");
    // this->preferences = new Preferences();
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        Serial.println("NVS partition was truncated and needs to be erased. Erasing...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    if (err != ESP_OK)
    {
        Serial.println("Failed to initialize NVS");
    }
    else
    {
        Serial.println("NVS OK");
    }
}

Settings::~Settings()
{
    // delete this->preferences;
    // this->preferences = nullptr;
}

void Settings::getValue(const char *key, char *value, size_t count)
{
    Serial.println("GET VALUE");
    Serial.println(key);
    if (this->preferences.begin("esp32-server-dashboard", false))
    {
        if (this->preferences.isKey(key))
        {
            this->preferences.getString(key, value, count);
            Serial.println(value);
        }
        else
        {
            Serial.println("NOT FOUND");
            if (count > 0)
            {
                value[0] = '\0';
            }
        }
        this->preferences.end();
    }
    else
    {
        Serial.println("ERROR init preferences");
    }
}

void Settings::setValue(const char *key, const char *value)
{
    Serial.println("SET VALUE");
    Serial.println(key);
    Serial.println(value);
    if (this->preferences.begin("esp32-server-dashboard", false))
    {
        if (this->preferences.putString(key, value))
        {
            Serial.println("OK");
        }
        else
        {
            Serial.println("ERROR");
        }
        this->preferences.end();
    }
    else
    {
        Serial.println("ERROR init preferences");
    }
    char a[100];
    this->getValue(key, a, 100);
}

void Settings::clear(void)
{
    if (this->preferences.begin("esp32-server-dashboard", false))
    {

        this->preferences.clear();
        this->preferences.end();
    }
    else
    {
        Serial.println("ERROR init preferences");
    }
}

void Settings::zap(void)
{
    nvs_flash_erase(); // erase the NVS partition and...
    nvs_flash_init();  // initialize the NVS partition.
}

void Settings::getWIFISSID(char *ssid, size_t count)
{
    this->getValue("WIFI_SSID", ssid, count);
}

void Settings::setWIFISSID(const char *ssid)
{
    this->setValue("WIFI_SSID", ssid);
}

void Settings::getWIFIPassword(char *password, size_t count)
{
    this->getValue("WIFI_PASSWORD", password, count);
}

void Settings::setWIFIPassword(const char *password)
{
    this->setValue("WIFI_PASSWORD", password);
}
