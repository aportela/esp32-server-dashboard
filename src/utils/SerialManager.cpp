#include "SerialManager.hpp"
#include "WifiManager.hpp"
#include <Arduino.h>
#include "Settings.hpp"

#define CMD_SET_WIFI_SSID "SET_WIFI_SSID "
#define CMD_SET_WIFI_PASSWORD "SET_WIFI_PASSWORD "
#define CMD_SET_MQTT_TELEGRAF_URI "SET_MQTT_TELEGRAF_URI "
#define CMD_SET_MQTT_TELEGRAF_GLOBAL_TOPIC "SET_MQTT_TELEGRAF_GLOBAL_TOPIC "

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
        else if (rx == "EXPORT_SETTINGS")
        {
            Serial.println("Exporting settings:");
            Serial.println("# EXPORTED SETTINGS BEGIN");
            Serial.println("CLEAR_SETTINGS");
            Settings *s = new Settings();
            char str[512] = {'\0'};
            s->getWIFISSID(str, sizeof(str));
            if (strlen(str) > 0)
            {
                Serial.print(CMD_SET_WIFI_SSID);
                Serial.println(str);
            }
            s->getWIFIPassword(str, sizeof(str));
            if (strlen(str) > 0)
            {
                Serial.print(CMD_SET_WIFI_PASSWORD);
                Serial.println(str);
            }
            s->getMQTTTelegrafURI(str, sizeof(str));
            if (strlen(str) > 0)
            {
                Serial.print(CMD_SET_MQTT_TELEGRAF_URI);
                Serial.println(str);
            }
            s->getMQTTTelegrafGlobalTopic(str, sizeof(str));
            if (strlen(str) > 0)
            {
                Serial.print(CMD_SET_MQTT_TELEGRAF_GLOBAL_TOPIC);
                Serial.println(str);
            }
            delete (s);
            Serial.println("REBOOT");
            Serial.println("# EXPORTED SETTINGS END");
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
        else if (rx.startsWith(CMD_SET_WIFI_SSID))
        {
            if (rx.length() > strlen(CMD_SET_WIFI_SSID))
            {
                String WiFiSSID = rx.substring(strlen(CMD_SET_WIFI_SSID));
                Serial.println("Received new WiFi SSID");
                Serial.println(WiFiSSID);
                Settings *s = new Settings();
                s->setWIFISSID(WiFiSSID.c_str());
                delete (s);
                Serial.println("WiFi SSID saved. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Received empty WiFi SSID");
                Settings *s = new Settings();
                s->setWIFISSID("");
                delete (s);
                Serial.println("WiFi SSID removed. Reboot REQUIRED");
            }
        }
        else if (rx.startsWith(CMD_SET_WIFI_PASSWORD))
        {
            if (rx.length() > strlen(CMD_SET_WIFI_PASSWORD))
            {
                String WiFiPassword = rx.substring(strlen(CMD_SET_WIFI_PASSWORD));
                Serial.println("Received new WiFi password");
                Serial.println(WiFiPassword);
                Settings *s = new Settings();
                s->setWIFIPassword(WiFiPassword.c_str());
                delete (s);
                Serial.println("WiFi password saved. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Received empty WiFi password");
                Settings *s = new Settings();
                s->setWIFIPassword("");
                delete (s);
                Serial.println("WiFi password removed. Reboot REQUIRED");
            }
        }
        else if (rx.startsWith(CMD_SET_MQTT_TELEGRAF_URI))
        {
            if (rx.length() > strlen(CMD_SET_MQTT_TELEGRAF_URI))
            {
                String MQTTTelegrafURI = rx.substring(strlen(CMD_SET_MQTT_TELEGRAF_URI));
                Serial.println("Received new MQTT Telegraf URI");
                Serial.println(MQTTTelegrafURI);
                Settings *s = new Settings();
                s->setMQTTTelegrafURI(MQTTTelegrafURI.c_str());
                delete (s);
                Serial.println("MQTT Telegraf URI saved. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Received empty MQTT Telegraf URI");
                Settings *s = new Settings();
                s->setMQTTTelegrafURI("");
                delete (s);
                Serial.println("MQTT Telegraf URI removed. Reboot REQUIRED");
            }
        }
        else if (rx.startsWith(CMD_SET_MQTT_TELEGRAF_GLOBAL_TOPIC))
        {
            if (rx.length() > strlen(CMD_SET_MQTT_TELEGRAF_GLOBAL_TOPIC))
            {
                String topic = rx.substring(strlen(CMD_SET_MQTT_TELEGRAF_GLOBAL_TOPIC));
                Serial.println("Received new MQTT Telegraf global topic");
                Serial.println(topic);
                Settings *s = new Settings();
                s->setMQTTTelegrafGlobalTopic(topic.c_str());
                delete (s);
                Serial.println("MQTT Telegraf global topic saved. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Received empty MQTT Telegraf global topic");
                Settings *s = new Settings();
                s->setMQTTTelegrafURI("");
                delete (s);
                Serial.println("MQTT Telegraf global topic removed. Reboot REQUIRED");
            }
        }
        else
        {
            Serial.printf("Unknown command %s\n", rx.c_str());
        }
    }
}
