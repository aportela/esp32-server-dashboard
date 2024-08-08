#include "SerialManager.hpp"
#include "WifiManager.hpp"
#include <Arduino.h>

const char *SerialCommandStr[]{
    "NONE",
    "UNKNOWN",
    "REBOOT",
    "CLEAR_SETTINGS",
    "EXPORT_SETTINGS",
    "CONNECT_WIFI",
    "DISCONNECT_WIFI",
    "SET_WIFI_SSID ",
    "SET_WIFI_PASSWORD ",
    "SET_MQTT_TELEGRAF_URI ",
    "SET_MQTT_TELEGRAF_GLOBAL_TOPIC ",
    "TOGGLE_SCREEN",
    "SET_MIN_CPU_TEMPERATURE ",
    "SET_MAX_CPU_TEMPERATURE ",
    "SET_MAX_DOWNLOAD_BYTES_BANDWITH ",
    "SET_MAX_UPLOAD_BYTES_BANDWITH ",
    "SET_NETWORK_INTERFACE_ID ",
    "SET_NETWORK_INTERFACE_NAME ",
};

SerialCommandCallback SerialManager::remoteCallback = nullptr;

void SerialManager::init(uint32_t speed, SerialCommandCallback callback)
{
    Serial.begin(speed);
    while (!Serial)
    {
        yield();
        delay(10);
    }
    SerialManager::setCallback(callback);
}

void SerialManager::setCallback(SerialCommandCallback callback)
{
    if (callback != nullptr)
    {
        SerialManager::remoteCallback = callback;
    }
}

void SerialManager::loop(void)
{
    while (Serial.available() > 0)
    {
        String rx = Serial.readStringUntil('\n');
        if (rx == SerialCommandStr[SERIAL_CMDT_REBOOT])
        {
            if (SerialManager::remoteCallback != nullptr)
            {
                SerialManager::remoteCallback(SERIAL_CMDT_REBOOT, nullptr);
            }
        }
        else if (rx == SerialCommandStr[SERIAL_CMDT_CLEAR_SETTINGS])
        {
            if (SerialManager::remoteCallback != nullptr)
            {
                SerialManager::remoteCallback(SERIAL_CMDT_CLEAR_SETTINGS, nullptr);
            }
        }
        else if (rx == SerialCommandStr[SERIAL_CMDT_EXPORT_SETTINGS])
        {
            if (SerialManager::remoteCallback != nullptr)
            {
                SerialManager::remoteCallback(SERIAL_CMDT_EXPORT_SETTINGS, nullptr);
            }
        }
        else if (rx == SerialCommandStr[SERIAL_CMDT_CONNECT_WIFI])
        {
            if (SerialManager::remoteCallback != nullptr)
            {
                SerialManager::remoteCallback(SERIAL_CMDT_CONNECT_WIFI, nullptr);
            }
        }
        else if (rx == SerialCommandStr[SERIAL_CMDT_DISCONNECT_WIFI])
        {
            if (SerialManager::remoteCallback != nullptr)
            {
                SerialManager::remoteCallback(SERIAL_CMDT_DISCONNECT_WIFI, nullptr);
            }
        }
        else if (rx.startsWith(SerialCommandStr[SERIAL_CMDT_SET_WIFI_SSID]))
        {
            uint16_t length = strlen(SerialCommandStr[SERIAL_CMDT_SET_WIFI_SSID]);
            if (rx.length() > length)
            {
                String WiFiSSID = rx.substring(length);
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_WIFI_SSID, WiFiSSID.c_str());
                }
            }
            else
            {
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_WIFI_SSID, nullptr);
                }
            }
        }
        else if (rx.startsWith(SerialCommandStr[SERIAL_CMDT_SET_WIFI_PASSWORD]))
        {
            uint16_t length = strlen(SerialCommandStr[SERIAL_CMDT_SET_WIFI_PASSWORD]);
            if (rx.length() > length)
            {
                String WiFiPassword = rx.substring(length);
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_WIFI_PASSWORD, WiFiPassword.c_str());
                }
            }
            else
            {
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_WIFI_PASSWORD, nullptr);
                }
            }
        }
        else if (rx.startsWith(SerialCommandStr[SERIAL_CMDT_SET_MQTT_TELEGRAF_URI]))
        {
            uint16_t length = strlen(SerialCommandStr[SERIAL_CMDT_SET_MQTT_TELEGRAF_URI]);
            if (rx.length() > length)
            {
                String MQTTTelegrafURI = rx.substring(length);
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_MQTT_TELEGRAF_URI, MQTTTelegrafURI.c_str());
                }
            }
            else
            {
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_MQTT_TELEGRAF_URI, nullptr);
                }
            }
        }
        else if (rx.startsWith(SerialCommandStr[SERIAL_CMDT_SET_MQTT_TELEGRAF_TOPIC]))
        {
            uint16_t length = strlen(SerialCommandStr[SERIAL_CMDT_SET_MQTT_TELEGRAF_TOPIC]);
            if (rx.length() > length)
            {
                String MQTTTelegrafTopic = rx.substring(length);
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_MQTT_TELEGRAF_TOPIC, MQTTTelegrafTopic.c_str());
                }
            }
            else
            {
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_MQTT_TELEGRAF_TOPIC, nullptr);
                }
            }
        }
        else if (rx == (SerialCommandStr[SERIAL_CMDT_TOGGLE_SCREEN]))
        {
            if (SerialManager::remoteCallback != nullptr)
            {
                SerialManager::remoteCallback(SERIAL_CMDT_TOGGLE_SCREEN, nullptr);
            }
        }
        else if (rx.startsWith(SerialCommandStr[SERIAL_CMDT_SET_MIN_CPU_TEMPERATURE]))
        {
            uint16_t length = strlen(SerialCommandStr[SERIAL_CMDT_SET_MIN_CPU_TEMPERATURE]);
            if (rx.length() > length)
            {
                String MinCPUTemperature = rx.substring(length);
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_MIN_CPU_TEMPERATURE, MinCPUTemperature.c_str());
                }
            }
            else
            {
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_MIN_CPU_TEMPERATURE, nullptr);
                }
            }
        }
        else if (rx.startsWith(SerialCommandStr[SERIAL_CMDT_SET_MAX_CPU_TEMPERATURE]))
        {
            uint16_t length = strlen(SerialCommandStr[SERIAL_CMDT_SET_MAX_CPU_TEMPERATURE]);
            if (rx.length() > length)
            {
                String MaxCPUTemperature = rx.substring(length);
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_MAX_CPU_TEMPERATURE, MaxCPUTemperature.c_str());
                }
            }
            else
            {
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_MAX_CPU_TEMPERATURE, nullptr);
                }
            }
        }
        else if (rx.startsWith(SerialCommandStr[SERIAL_CMDT_SET_MAX_DOWNLOAD_BYTES_BANDWIDTH]))
        {
            uint16_t length = strlen(SerialCommandStr[SERIAL_CMDT_SET_MAX_DOWNLOAD_BYTES_BANDWIDTH]);
            if (rx.length() > length)
            {
                String MaxDownloadBytesBandwidth = rx.substring(length);
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_MAX_DOWNLOAD_BYTES_BANDWIDTH, MaxDownloadBytesBandwidth.c_str());
                }
            }
            else
            {
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_MAX_DOWNLOAD_BYTES_BANDWIDTH, nullptr);
                }
            }
        }
        else if (rx.startsWith(SerialCommandStr[SERIAL_CMDT_SET_MAX_UPLOAD_BYTES_BANDWIDTH]))
        {
            uint16_t length = strlen(SerialCommandStr[SERIAL_CMDT_SET_MAX_UPLOAD_BYTES_BANDWIDTH]);
            if (rx.length() > length)
            {
                String MaxUploadBytesBandwidth = rx.substring(length);
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_MAX_UPLOAD_BYTES_BANDWIDTH, MaxUploadBytesBandwidth.c_str());
                }
            }
            else
            {
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_MAX_UPLOAD_BYTES_BANDWIDTH, nullptr);
                }
            }
        }
        else if (rx.startsWith(SerialCommandStr[SERIAL_CMDT_SET_NETWORK_INTERFACE_ID]))
        {
            uint16_t length = strlen(SerialCommandStr[SERIAL_CMDT_SET_NETWORK_INTERFACE_ID]);
            if (rx.length() > length)
            {
                String Id = rx.substring(length);
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_NETWORK_INTERFACE_ID, Id.c_str());
                }
            }
            else
            {
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_NETWORK_INTERFACE_ID, nullptr);
                }
            }
        }
        else if (rx.startsWith(SerialCommandStr[SERIAL_CMDT_SET_NETWORK_INTERFACE_NAME]))
        {
            uint16_t length = strlen(SerialCommandStr[SERIAL_CMDT_SET_NETWORK_INTERFACE_NAME]);
            if (rx.length() > length)
            {
                String Name = rx.substring(length);
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_NETWORK_INTERFACE_NAME, Name.c_str());
                }
            }
            else
            {
                if (SerialManager::remoteCallback != nullptr)
                {
                    SerialManager::remoteCallback(SERIAL_CMDT_SET_NETWORK_INTERFACE_NAME, nullptr);
                }
            }
        }
        else
        {
            if (SerialManager::remoteCallback != nullptr)
            {
                SerialManager::remoteCallback(SERIAL_CMDT_UNKNOWN, rx.c_str());
            }
        }
    }
}
