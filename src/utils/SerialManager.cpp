#include "SerialManager.hpp"
#include "WifiManager.hpp"
#include <Arduino.h>
#include "../settings/Settings.hpp"

// Settings SerialManager::settings = nullptr;

void SerialManager::init(uint32_t speed)
{
    Serial.begin(speed);
    while (!Serial)
    {
        yield();
        delay(10);
    }
}

void SerialManager::loop(void)
{
    while (Serial.available() > 0)
    {
        String rx = Serial.readStringUntil('\n');
        if (rx == "REBOOT")
        {
            Serial.println("Rebooting");
            ESP.restart();
        }
        else if (rx == "CLEAR_SETTINGS")
        {
            Serial.println("Reseting settings");
            // settings->clear();
        }
        else if (rx == "CONNECT_WIFI")
        {
            Serial.println("Connecting WIFI");
            WifiManager::connect(false);
        }
        else if (rx == "DISCONNECT_WIFI")
        {
            Serial.println("Disconnecting WIFI");
            WifiManager::disconnect();
        }
        else if (rx.startsWith("SET_WIFI_SSID ") && rx.length() > 14)
        {
            String rxSSID = rx.substring(14);
            if (rxSSID.length() > 0)
            {
                Serial.println("Received new SSID");
                Serial.println(rxSSID);
                // SerialManager::settings->setWIFISSID(rxSSID.c_str());
            }
        }
        else if (rx.startsWith("SET_WIFI_PASSWORD ") && rx.length() > 18)
        {
            String rxPassword = rx.substring(18);
            if (rxPassword.length() > 0)
            {
                Serial.println("Received new password");
                Serial.println(rxPassword);
                Settings *s = new Settings();
                s->setWIFIPassword(rxPassword.c_str());
            }
        }
        else
        {
            Serial.printf("Unknown cmd %s\n", rx.c_str());
        }
    }
}
