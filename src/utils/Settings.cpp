#include "Settings.hpp"

// WARNING: namespaces with length > 15 WILL NOT OPEN NVS CORRECTLY
#define NAMESPACE "esp32_srv_dashb"

// WARNING: ALSO keys with length > 15 DO NOT WORK
#define KEY_WIFI_SSID "WIFI_SSID"
#define KEY_WIFI_PASSWORD "WIFI_PASSWORD"
#define KEY_MQTT_TELEGRAF_URI "SRC_MQTT_URI"
#define KEY_MQTT_TELEGRAF_GLOBAL_TOPIC "SRC_MQTT_TOPIC"
#define KEY_TOTAL_MEMORY_BYTES "T_MEM_BYTES"

Preferences Settings::preferences;

uint64_t Settings::getBigUnsignedIntegerValue(const char *key, uint64_t defaultValue)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        if (Settings::preferences.isKey(key))
        {
            return (Settings::preferences.getULong64(key, defaultValue));
        }
        else
        {
            return (defaultValue);
        }
    }
    else
    {
        return (defaultValue);
    }
}

void Settings::setBigIntegerValue(const char *key, uint64_t value)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        Serial.println("SI");
        if (Settings::preferences.putULong64(key, value))
        {
            Serial.println("PutYlong ok");
        }
        else
        {
            Serial.println("PutYlong error");
        }
        Settings::preferences.end();
    }
    else
    {
        Serial.println("NO");
    }
}

void Settings::getValue(const char *key, char *value, size_t count)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        if (Settings::preferences.isKey(key))
        {
            Settings::preferences.getString(key, value, count);
        }
        else
        {
            if (count > 0)
            {
                value[0] = '\0';
            }
        }
        Settings::preferences.end();
    }
}

void Settings::setValue(const char *key, const char *value)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        if (Settings::preferences.putString(key, value))
        {
        }
        else
        {
        }
        Settings::preferences.end();
    }
}

void Settings::deleteKey(const char *key)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        Settings::preferences.remove(key);
    }
}

void Settings::clear(void)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        Settings::preferences.clear();
        Settings::preferences.end();
    }
}

void Settings::getWIFISSID(char *ssid, size_t count)
{
    Settings::getValue(KEY_WIFI_SSID, ssid, count);
}

void Settings::setWIFISSID(const char *ssid)
{
    if (strlen(ssid) > 0)
    {
        Settings::setValue(KEY_WIFI_SSID, ssid);
    }
    else
    {
        Settings::deleteKey(KEY_WIFI_SSID);
    }
}

void Settings::getWIFIPassword(char *password, size_t count)
{
    Settings::getValue(KEY_WIFI_PASSWORD, password, count);
}

void Settings::setWIFIPassword(const char *password)
{
    if (strlen(password) > 0)
    {
        Settings::setValue(KEY_WIFI_PASSWORD, password);
    }
    else
    {
        Settings::deleteKey(KEY_WIFI_PASSWORD);
    }
}

void Settings::getMQTTTelegrafURI(char *uri, size_t count)
{
    Settings::getValue(KEY_MQTT_TELEGRAF_URI, uri, count);
}

void Settings::setMQTTTelegrafURI(const char *uri)
{
    if (strlen(uri) > 0)
    {
        Settings::setValue(KEY_MQTT_TELEGRAF_URI, uri);
    }
    else
    {
        Settings::deleteKey(KEY_MQTT_TELEGRAF_URI);
    }
}

void Settings::getMQTTTelegrafGlobalTopic(char *topic, size_t count)
{
    Settings::getValue(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC, topic, count);
}

void Settings::setMQTTTelegrafGlobalTopic(const char *topic)
{
    if (strlen(topic) > 0)
    {
        Settings::setValue(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC, topic);
    }
    else
    {
        Settings::deleteKey(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC);
    }
}

uint64_t Settings::getTotalMemoryBytes()
{
    return (Settings::getBigUnsignedIntegerValue(KEY_TOTAL_MEMORY_BYTES, 6));
}

void Settings::setTotalMemoryBytes(uint64_t totalBytes)
{
    if (totalBytes > 0)
    {
        Serial.printf("Settings::setTotalMemoryBytes total: %" PRIu64 "\n", totalBytes);
        Settings::setBigIntegerValue(KEY_TOTAL_MEMORY_BYTES, totalBytes);
    }
    else
    {
        Settings::deleteKey(KEY_TOTAL_MEMORY_BYTES);
    }
}
