#include "Settings.hpp"

// WARNING: namespaces with length > 15 WILL NOT OPEN NVS CORRECTLY
#define NAMESPACE "esp32_srv_dashb"

// WARNING: ALSO keys with length > 15 DO NOT WORK
#define KEY_WIFI_SSID "WIFI_SSID"
#define KEY_WIFI_PASSWORD "WIFI_PASSWORD"
#define KEY_MQTT_TELEGRAF_URI "SRC_MQTT_URI"
#define KEY_MQTT_TELEGRAF_GLOBAL_TOPIC "SRC_MQTT_TOPIC"
#define KEY_TOTAL_DOWNLOAD_BANDWIDTH_BYTES "T_D_BW_BYTES"
#define KEY_TOTAL_UPLOAD_BANDWIDTH_BYTES "T_U_BW_BYTES"
#define KEY_NET_IFACE_ID "NET_IFACE_ID"

Settings::Settings(void)
{
    this->preferences = new Preferences();
}

Settings::~Settings(void)
{
    if (this->preferences != nullptr)
    {
        delete this->preferences;
        this->preferences = nullptr;
    }
}

int8_t Settings::getSmallSignedIntegerValue(const char *key, int8_t defaultValue)
{
    if (this->preferences->begin(NAMESPACE, false))
    {
        uint64_t value = defaultValue;
        if (this->preferences->isKey(key))
        {
            value = this->preferences->getChar(key, defaultValue);
        }
        this->preferences->end();
        return (value);
    }
    else
    {
        return (defaultValue);
    }
}

bool Settings::setSmallSignedIntegerValue(const char *key, int8_t value)
{
    if (this->preferences->begin(NAMESPACE, false))
    {
        bool saved = this->preferences->putChar(key, value) == sizeof(value);
        this->preferences->end();
        return (saved);
    }
    else
    {
        return (false);
    }
}

uint64_t Settings::getBigUnsignedIntegerValue(const char *key, uint64_t defaultValue)
{
    if (this->preferences->begin(NAMESPACE, false))
    {
        uint64_t value = defaultValue;
        if (this->preferences->isKey(key))
        {
            value = this->preferences->getULong64(key, defaultValue);
        }
        this->preferences->end();
        return (value);
    }
    else
    {
        return (defaultValue);
    }
}

bool Settings::setBigUnsigedIntegerValue(const char *key, uint64_t value)
{
    if (this->preferences->begin(NAMESPACE, false))
    {
        bool saved = this->preferences->putULong64(key, value) == sizeof(value);
        this->preferences->end();
        return (saved);
    }
    else
    {
        return (false);
    }
}

float Settings::getFloatValue(const char *key, float defaultValue)
{
    if (this->preferences->begin(NAMESPACE, false))
    {
        float value = defaultValue;
        if (this->preferences->isKey(key))
        {
            value = this->preferences->getFloat(key, defaultValue);
        }
        this->preferences->end();
        return (value);
    }
    else
    {
        return (defaultValue);
    }
}

bool Settings::setFloatValue(const char *key, float value)
{
    if (this->preferences->begin(NAMESPACE, false))
    {
        bool saved = this->preferences->putFloat(key, value) == sizeof(value);
        this->preferences->end();
        return (saved);
    }
    else
    {
        return (false);
    }
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

bool Settings::setValue(const char *key, const char *value)
{
    if (this->preferences->begin(NAMESPACE, false))
    {
        bool saved = this->preferences->putString(key, value) == strlen(value);
        this->preferences->end();
        return (saved);
    }
    else
    {
        return (false);
    }
}

bool Settings::deleteKey(const char *key)
{
    if (this->preferences->begin(NAMESPACE, false))
    {
        bool removed = false;
        if (this->preferences->isKey(key))
        {
            this->preferences->remove(key);
            removed = true;
        }
        this->preferences->end();
        return (removed);
    }
    else
    {
        return (false);
    }
}

bool Settings::clear(void)
{
    if (this->preferences->begin(NAMESPACE, false))
    {
        bool cleared = this->preferences->clear();
        this->preferences->end();
        return (cleared);
    }
    else
    {
        return (false);
    }
}

void Settings::getWIFISSID(char *ssid, size_t count)
{
    this->getValue(KEY_WIFI_SSID, ssid, count);
}

bool Settings::setWIFISSID(const char *ssid)
{
    if (strlen(ssid) > 0)
    {
        return (this->setValue(KEY_WIFI_SSID, ssid));
    }
    else
    {
        return (this->deleteKey(KEY_WIFI_SSID));
    }
}

void Settings::getWIFIPassword(char *password, size_t count)
{
    this->getValue(KEY_WIFI_PASSWORD, password, count);
}

bool Settings::setWIFIPassword(const char *password)
{
    if (strlen(password) > 0)
    {
        return (this->setValue(KEY_WIFI_PASSWORD, password));
    }
    else
    {
        return (this->deleteKey(KEY_WIFI_PASSWORD));
    }
}

void Settings::getMQTTTelegrafURI(char *uri, size_t count)
{
    this->getValue(KEY_MQTT_TELEGRAF_URI, uri, count);
}

bool Settings::setMQTTTelegrafURI(const char *uri)
{
    if (strlen(uri) > 0)
    {
        return (this->setValue(KEY_MQTT_TELEGRAF_URI, uri));
    }
    else
    {
        return (this->deleteKey(KEY_MQTT_TELEGRAF_URI));
    }
}

void Settings::getMQTTTelegrafGlobalTopic(char *topic, size_t count)
{
    this->getValue(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC, topic, count);
}

bool Settings::setMQTTTelegrafGlobalTopic(const char *topic)
{
    if (strlen(topic) > 0)
    {
        return (this->setValue(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC, topic));
    }
    else
    {
        return (this->deleteKey(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC));
    }
}

uint64_t Settings::getMaxDownloadBandwidthBytes()
{
    return (this->getBigUnsignedIntegerValue(KEY_TOTAL_DOWNLOAD_BANDWIDTH_BYTES, 0));
}

bool Settings::setMaxDownloadBandwidthBytes(uint64_t totalBytes)
{
    if (totalBytes > 0)
    {
        return (this->setBigUnsigedIntegerValue(KEY_TOTAL_DOWNLOAD_BANDWIDTH_BYTES, totalBytes));
    }
    else
    {
        return (this->deleteKey(KEY_TOTAL_DOWNLOAD_BANDWIDTH_BYTES));
    }
}

uint64_t Settings::getMaxUploadBandwidthBytes()
{
    return (this->getBigUnsignedIntegerValue(KEY_TOTAL_UPLOAD_BANDWIDTH_BYTES, 0));
}

bool Settings::setMaxUploadBandwidthBytes(uint64_t totalBytes)
{
    if (totalBytes > 0)
    {
        return (this->setBigUnsigedIntegerValue(KEY_TOTAL_UPLOAD_BANDWIDTH_BYTES, totalBytes));
    }
    else
    {
        return (this->deleteKey(KEY_TOTAL_UPLOAD_BANDWIDTH_BYTES));
    }
}

void Settings::getNetworkInterfaceId(char *id, size_t count)
{
    this->getValue(KEY_NET_IFACE_ID, id, count);
}

bool Settings::setNetworkInterfaceId(const char *id)
{
    if (strlen(id) > 0)
    {
        return (this->setValue(KEY_NET_IFACE_ID, id));
    }
    else
    {
        return (this->deleteKey(KEY_NET_IFACE_ID));
    }
}
