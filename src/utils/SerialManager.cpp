#include "SerialManager.hpp"
#include "WifiManager.hpp"
#include <Arduino.h>
#include "Settings.hpp"

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
            Settings *s = new Settings();
            s->clear();
            delete (s);
            Serial.println("Settings cleared. Reboot REQUIRED");
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
                Settings *s = new Settings();
                s->setWIFISSID(rxSSID.c_str());
                delete (s);
                Serial.println("WIFI SSID saved. Reboot REQUIRED");
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
                delete (s);
                Serial.println("WIFI Password saved. Reboot REQUIRED");
            }
        }
        else if (rx.startsWith("SET_MQTT_TELEGRAF_URI ") && rx.length() > 22)
        {
            String uri = rx.substring(22);
            if (uri.length() > 0)
            {
                Serial.println("Received new MQTT Telegraf URI");
                Serial.println(uri);
                Settings *s = new Settings();
                s->setMQTTTelegrafURI(uri.c_str());
                delete (s);
                Serial.println("MQTT Telegraf URI saved. Reboot REQUIRED");
            }
        }
        else if (rx.startsWith("SET_MQTT_TELEGRAF_GLOBAL_TOPIC ") && rx.length() > 31)
        {
            String topic = rx.substring(31);
            if (topic.length() > 0)
            {
                Serial.println("Received new MQTT Telegraf global topic");
                Serial.println(topic);
                Settings *s = new Settings();
                s->setWIFIPassword(topic.c_str());
                delete (s);
                Serial.println("MQTT Telegraf global topic saved. Reboot REQUIRED");
            }
        }
        else
        {
            Serial.printf("Unknown cmd %s\n", rx.c_str());
        }
    }
}
