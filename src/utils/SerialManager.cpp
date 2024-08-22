#include "SerialManager.hpp"
#include <Arduino.h>

namespace aportela::microcontroller::utils
{
    // WARNINT: THIS NEEDS TO BE "PAIRED" WITH SerialManagerCommand defined on SerialManager.hpp
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
        "SET_MAX_DOWNLOAD_BYTES_BANDWITH ",
        "SET_MAX_UPLOAD_BYTES_BANDWITH ",
        "SET_NETWORK_INTERFACE_ID ",
        "SET_HOSTNAME ",
        "SET_DEFAULT_SCREEN ",
    };

    SerialCommandCallback SerialManager::commandReceivedCallback = nullptr;

    void SerialManager::init(uint32_t speed, SerialCommandCallback callback)
    {
        Serial.begin(speed);
        while (!Serial)
        {
            yield();
            delay(10);
        }
        SerialManager::onCommandReceived(callback);
    }

    void SerialManager::onCommandReceived(SerialCommandCallback callback)
    {
        if (callback != nullptr)
        {
            SerialManager::commandReceivedCallback = callback;
        }
    }

    void SerialManager::loop(void)
    {
        while (Serial.available() > 0)
        {
            String rx = Serial.readStringUntil('\n');
            if (rx == SerialCommandStr[SerialManagerCommand_REBOOT])
            {
                if (SerialManager::commandReceivedCallback != nullptr)
                {
                    SerialManager::commandReceivedCallback(SerialManagerCommand_REBOOT, nullptr);
                }
            }
            else if (rx == SerialCommandStr[SerialManagerCommand_CLEAR_SETTINGS])
            {
                if (SerialManager::commandReceivedCallback != nullptr)
                {
                    SerialManager::commandReceivedCallback(SerialManagerCommand_CLEAR_SETTINGS, nullptr);
                }
            }
            else if (rx == SerialCommandStr[SerialManagerCommand_EXPORT_SETTINGS])
            {
                if (SerialManager::commandReceivedCallback != nullptr)
                {
                    SerialManager::commandReceivedCallback(SerialManagerCommand_EXPORT_SETTINGS, nullptr);
                }
            }
            else if (rx == SerialCommandStr[SerialManagerCommand_CONNECT_WIFI])
            {
                if (SerialManager::commandReceivedCallback != nullptr)
                {
                    SerialManager::commandReceivedCallback(SerialManagerCommand_CONNECT_WIFI, nullptr);
                }
            }
            else if (rx == SerialCommandStr[SerialManagerCommand_DISCONNECT_WIFI])
            {
                if (SerialManager::commandReceivedCallback != nullptr)
                {
                    SerialManager::commandReceivedCallback(SerialManagerCommand_DISCONNECT_WIFI, nullptr);
                }
            }
            else if (rx.startsWith(SerialCommandStr[SerialManagerCommand_SET_WIFI_SSID]))
            {
                uint16_t length = strlen(SerialCommandStr[SerialManagerCommand_SET_WIFI_SSID]);
                if (rx.length() > length)
                {
                    String WiFiSSID = rx.substring(length);
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_WIFI_SSID, WiFiSSID.c_str());
                    }
                }
                else
                {
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_WIFI_SSID, nullptr);
                    }
                }
            }
            else if (rx.startsWith(SerialCommandStr[SerialManagerCommand_SET_WIFI_PASSWORD]))
            {
                uint16_t length = strlen(SerialCommandStr[SerialManagerCommand_SET_WIFI_PASSWORD]);
                if (rx.length() > length)
                {
                    String WiFiPassword = rx.substring(length);
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_WIFI_PASSWORD, WiFiPassword.c_str());
                    }
                }
                else
                {
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_WIFI_PASSWORD, nullptr);
                    }
                }
            }
            else if (rx.startsWith(SerialCommandStr[SerialManagerCommand_SET_MQTT_TELEGRAF_URI]))
            {
                uint16_t length = strlen(SerialCommandStr[SerialManagerCommand_SET_MQTT_TELEGRAF_URI]);
                if (rx.length() > length)
                {
                    String MQTTTelegrafURI = rx.substring(length);
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_MQTT_TELEGRAF_URI, MQTTTelegrafURI.c_str());
                    }
                }
                else
                {
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_MQTT_TELEGRAF_URI, nullptr);
                    }
                }
            }
            else if (rx.startsWith(SerialCommandStr[SerialManagerCommand_SET_MQTT_TELEGRAF_TOPIC]))
            {
                uint16_t length = strlen(SerialCommandStr[SerialManagerCommand_SET_MQTT_TELEGRAF_TOPIC]);
                if (rx.length() > length)
                {
                    String MQTTTelegrafTopic = rx.substring(length);
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_MQTT_TELEGRAF_TOPIC, MQTTTelegrafTopic.c_str());
                    }
                }
                else
                {
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_MQTT_TELEGRAF_TOPIC, nullptr);
                    }
                }
            }
            else if (rx == (SerialCommandStr[SerialManagerCommand_TOGGLE_SCREEN]))
            {
                if (SerialManager::commandReceivedCallback != nullptr)
                {
                    SerialManager::commandReceivedCallback(SerialManagerCommand_TOGGLE_SCREEN, nullptr);
                }
            }
            else if (rx.startsWith(SerialCommandStr[SerialManagerCommand_SET_MAX_DOWNLOAD_BYTES_BANDWIDTH]))
            {
                uint16_t length = strlen(SerialCommandStr[SerialManagerCommand_SET_MAX_DOWNLOAD_BYTES_BANDWIDTH]);
                if (rx.length() > length)
                {
                    String MaxDownloadBytesBandwidth = rx.substring(length);
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_MAX_DOWNLOAD_BYTES_BANDWIDTH, MaxDownloadBytesBandwidth.c_str());
                    }
                }
                else
                {
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_MAX_DOWNLOAD_BYTES_BANDWIDTH, nullptr);
                    }
                }
            }
            else if (rx.startsWith(SerialCommandStr[SerialManagerCommand_SET_MAX_UPLOAD_BYTES_BANDWIDTH]))
            {
                uint16_t length = strlen(SerialCommandStr[SerialManagerCommand_SET_MAX_UPLOAD_BYTES_BANDWIDTH]);
                if (rx.length() > length)
                {
                    String MaxUploadBytesBandwidth = rx.substring(length);
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_MAX_UPLOAD_BYTES_BANDWIDTH, MaxUploadBytesBandwidth.c_str());
                    }
                }
                else
                {
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_MAX_UPLOAD_BYTES_BANDWIDTH, nullptr);
                    }
                }
            }
            else if (rx.startsWith(SerialCommandStr[SerialManagerCommand_SET_NETWORK_INTERFACE_ID]))
            {
                uint16_t length = strlen(SerialCommandStr[SerialManagerCommand_SET_NETWORK_INTERFACE_ID]);
                if (rx.length() > length)
                {
                    String Id = rx.substring(length);
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_NETWORK_INTERFACE_ID, Id.c_str());
                    }
                }
                else
                {
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_NETWORK_INTERFACE_ID, nullptr);
                    }
                }
            }
            else if (rx.startsWith(SerialCommandStr[SerialManagerCommand_SET_HOSTNAME]))
            {
                uint16_t length = strlen(SerialCommandStr[SerialManagerCommand_SET_HOSTNAME]);
                if (rx.length() > length)
                {
                    String Hostname = rx.substring(length);
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_HOSTNAME, Hostname.c_str());
                    }
                }
                else
                {
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_HOSTNAME, nullptr);
                    }
                }
            }
            else if (rx.startsWith(SerialCommandStr[SerialManagerCommand_SET_DEFAULT_SCREEN]))
            {
                uint16_t length = strlen(SerialCommandStr[SerialManagerCommand_SET_DEFAULT_SCREEN]);
                if (rx.length() > length)
                {
                    String defaultScreen = rx.substring(length);
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_DEFAULT_SCREEN, defaultScreen.c_str());
                    }
                }
                else
                {
                    if (SerialManager::commandReceivedCallback != nullptr)
                    {
                        SerialManager::commandReceivedCallback(SerialManagerCommand_SET_DEFAULT_SCREEN, nullptr);
                    }
                }
            }
            else
            {
                if (SerialManager::commandReceivedCallback != nullptr)
                {
                    SerialManager::commandReceivedCallback(SerialManagerCommand_UNKNOWN, rx.c_str());
                }
            }
        }
    }
}
