#include "Settings.hpp"

// WARNING: namespaces with length > 15 WILL NOT OPEN NVS CORRECTLY
#define NAMESPACE "esp32_srv_dashb"

// WARNING: ALSO keys with length > 15 DO NOT WORK
#define KEY_WIFI_SSID "WIFI_SSID"
#define KEY_WIFI_PASSWORD "WIFI_PASSWORD"
#define KEY_MQTT_TELEGRAF_URI "SRC_MQTT_URI"
#define KEY_MQTT_TELEGRAF_GLOBAL_TOPIC "SRC_MQTT_TOPIC"
#define KEY_TOTAL_MEMORY_BYTES "T_MEM_BYTES"
#define KEY_MIN_CPU_TEMPERATURE "MIN_CPU_TEMP"
#define KEY_MAX_CPU_TEMPERATURE "MAX_CPU_TEMP"
#define KEY_TOTAL_DOWNLOAD_BANDWIDTH_BYTES "T_D_BW_BYTES"
#define KEY_TOTAL_UPLOAD_BANDWIDTH_BYTES "T_U_BW_BYTES"
#define KEY_NET_IFACE_ID "NET_IFACE_ID"
#define KEY_NET_IFACE_NAME "NET_IFACE_NAME"

Preferences Settings::preferences;

int8_t Settings::getSmallSignedIntegerValue(const char *key, int8_t defaultValue)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        uint64_t value = defaultValue;
        if (Settings::preferences.isKey(key))
        {
            value = Settings::preferences.getChar(key, defaultValue);
        }
        Settings::preferences.end();
        return (value);
    }
    else
    {
        return (defaultValue);
    }
}

bool Settings::setSmallSignedIntegerValue(const char *key, int8_t value)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        bool saved = Settings::preferences.putChar(key, value) == sizeof(value);
        Settings::preferences.end();
        return (saved);
    }
    else
    {
        return (false);
    }
}

uint64_t Settings::getBigUnsignedIntegerValue(const char *key, uint64_t defaultValue)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        uint64_t value = defaultValue;
        if (Settings::preferences.isKey(key))
        {
            value = Settings::preferences.getULong64(key, defaultValue);
        }
        Settings::preferences.end();
        return (value);
    }
    else
    {
        return (defaultValue);
    }
}

bool Settings::setBigUnsigedIntegerValue(const char *key, uint64_t value)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        bool saved = Settings::preferences.putULong64(key, value) == sizeof(value);
        Settings::preferences.end();
        return (saved);
    }
    else
    {
        return (false);
    }
}

float Settings::getFloatValue(const char *key, float defaultValue)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        float value = defaultValue;
        if (Settings::preferences.isKey(key))
        {
            value = Settings::preferences.getFloat(key, defaultValue);
        }
        Settings::preferences.end();
        return (value);
    }
    else
    {
        return (defaultValue);
    }
}

bool Settings::setFloatValue(const char *key, float value)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        bool saved = Settings::preferences.putFloat(key, value) == sizeof(value);
        Settings::preferences.end();
        return (saved);
    }
    else
    {
        return (false);
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

bool Settings::setValue(const char *key, const char *value)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        bool saved = Settings::preferences.putString(key, value) == strlen(value);
        Settings::preferences.end();
        return (saved);
    }
    else
    {
        return (false);
    }
}

bool Settings::deleteKey(const char *key)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        bool removed = false;
        if (Settings::preferences.isKey(key))
        {
            Settings::preferences.remove(key);
            removed = true;
        }
        Settings::preferences.end();
        return (removed);
    }
    else
    {
        return (false);
    }
}

bool Settings::clear(void)
{
    if (Settings::preferences.begin(NAMESPACE, false))
    {
        bool cleared = Settings::preferences.clear();
        Settings::preferences.end();
        return (cleared);
    }
    else
    {
        return (false);
    }
}

void Settings::getWIFISSID(char *ssid, size_t count)
{
    Settings::getValue(KEY_WIFI_SSID, ssid, count);
}

bool Settings::setWIFISSID(const char *ssid)
{
    if (strlen(ssid) > 0)
    {
        return (Settings::setValue(KEY_WIFI_SSID, ssid));
    }
    else
    {
        return (Settings::deleteKey(KEY_WIFI_SSID));
    }
}

void Settings::getWIFIPassword(char *password, size_t count)
{
    Settings::getValue(KEY_WIFI_PASSWORD, password, count);
}

bool Settings::setWIFIPassword(const char *password)
{
    if (strlen(password) > 0)
    {
        return (Settings::setValue(KEY_WIFI_PASSWORD, password));
    }
    else
    {
        return (Settings::deleteKey(KEY_WIFI_PASSWORD));
    }
}

void Settings::getMQTTTelegrafURI(char *uri, size_t count)
{
    Settings::getValue(KEY_MQTT_TELEGRAF_URI, uri, count);
}

bool Settings::setMQTTTelegrafURI(const char *uri)
{
    if (strlen(uri) > 0)
    {
        return (Settings::setValue(KEY_MQTT_TELEGRAF_URI, uri));
    }
    else
    {
        return (Settings::deleteKey(KEY_MQTT_TELEGRAF_URI));
    }
}

void Settings::getMQTTTelegrafGlobalTopic(char *topic, size_t count)
{
    Settings::getValue(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC, topic, count);
}

bool Settings::setMQTTTelegrafGlobalTopic(const char *topic)
{
    if (strlen(topic) > 0)
    {
        return (Settings::setValue(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC, topic));
    }
    else
    {
        return (Settings::deleteKey(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC));
    }
}

uint64_t Settings::getTotalMemoryBytes()
{
    return (Settings::getBigUnsignedIntegerValue(KEY_TOTAL_MEMORY_BYTES, 0));
}

bool Settings::setTotalMemoryBytes(uint64_t totalBytes)
{
    if (totalBytes > 0)
    {
        return (Settings::setBigUnsigedIntegerValue(KEY_TOTAL_MEMORY_BYTES, totalBytes));
    }
    else
    {
        return (Settings::deleteKey(KEY_TOTAL_MEMORY_BYTES));
    }
}

float Settings::getMinCPUTemperature()
{
    return (Settings::getFloatValue(KEY_MIN_CPU_TEMPERATURE, 0.0f));
}

bool Settings::setMinCPUTemperature(float celsius)
{
    if (celsius >= -128 && celsius <= 127)
    {
        return (Settings::setFloatValue(KEY_MIN_CPU_TEMPERATURE, celsius));
    }
    else
    {
        return (Settings::deleteKey(KEY_MIN_CPU_TEMPERATURE));
    }
}

float Settings::getMaxCPUTemperature()
{
    return (Settings::getFloatValue(KEY_MAX_CPU_TEMPERATURE, 0.0f));
}

bool Settings::setMaxCPUTemperature(float celsius)
{
    if (celsius >= -128 && celsius <= 127)
    {
        return (Settings::setFloatValue(KEY_MAX_CPU_TEMPERATURE, celsius));
    }
    else
    {
        return (Settings::deleteKey(KEY_MAX_CPU_TEMPERATURE));
    }
}

uint64_t Settings::getMaxDownloadBandwidthBytes()
{
    return (Settings::getBigUnsignedIntegerValue(KEY_TOTAL_DOWNLOAD_BANDWIDTH_BYTES, 0));
}

bool Settings::setMaxDownloadBandwidthBytes(uint64_t totalBytes)
{
    if (totalBytes > 0)
    {
        return (Settings::setBigUnsigedIntegerValue(KEY_TOTAL_DOWNLOAD_BANDWIDTH_BYTES, totalBytes));
    }
    else
    {
        return (Settings::deleteKey(KEY_TOTAL_DOWNLOAD_BANDWIDTH_BYTES));
    }
}

uint64_t Settings::getMaxUploadBandwidthBytes()
{
    return (Settings::getBigUnsignedIntegerValue(KEY_TOTAL_UPLOAD_BANDWIDTH_BYTES, 0));
}

bool Settings::setMaxUploadBandwidthBytes(uint64_t totalBytes)
{
    if (totalBytes > 0)
    {
        return (Settings::setBigUnsigedIntegerValue(KEY_TOTAL_UPLOAD_BANDWIDTH_BYTES, totalBytes));
    }
    else
    {
        return (Settings::deleteKey(KEY_TOTAL_UPLOAD_BANDWIDTH_BYTES));
    }
}

void Settings::getNetworkInterfaceId(char *id, size_t count)
{
    Settings::getValue(KEY_NET_IFACE_ID, id, count);
}

bool Settings::setNetworkInterfaceId(const char *id)
{
    if (strlen(id) > 0)
    {
        return (Settings::setValue(KEY_NET_IFACE_ID, id));
    }
    else
    {
        return (Settings::deleteKey(KEY_NET_IFACE_ID));
    }
}

void Settings::getNetworkInterfaceName(char *name, size_t count)
{
    Settings::getValue(KEY_NET_IFACE_NAME, name, count);
}

bool Settings::setNetworkInterfaceName(const char *name)
{
    if (strlen(name) > 0)
    {
        return (Settings::setValue(KEY_NET_IFACE_NAME, name));
    }
    else
    {
        return (Settings::deleteKey(KEY_NET_IFACE_NAME));
    }
}
