#include "Settings.hpp"

// WARNING: namespaces with length > 15 WILL NOT OPEN NVS CORRECTLY
#define NAMESPACE "esp32_srv_dashb"

// WARNING: ALSO keys with length > 15 DO NOT WORK
#define KEY_WIFI_SSID "WIFI_SSID"
#define KEY_WIFI_PASSWORD "WIFI_PASSWORD"
#define KEY_MQTT_TELEGRAF_URI "SRC_MQTT_URI"
#define KEY_MQTT_TELEGRAF_GLOBAL_TOPIC "SRC_MQTT_TOPIC"
#define KEY_TOTAL_MEMORY_BYTES "T_MEM_BYTES"

Settings::Settings(void)
{
    this->preferences = new Preferences();
}

Settings::~Settings()
{
    delete this->preferences;
    this->preferences = nullptr;
}

uint64_t Settings::getBigUnsignedIntegerValue(const char *key, uint64_t defaultValue)
{
    if (this->preferences->begin(NAMESPACE, false))
    {
        if (this->preferences->isKey(key))
        {
            return (this->preferences->getULong64(key, defaultValue));
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
    if (this->preferences->begin(NAMESPACE, false))
    {
        if (this->preferences->putULong64(key, value))
        {
        }
        else
        {
        }
        this->preferences->end();
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

void Settings::deleteKey(const char *key)
{
    if (this->preferences->begin(NAMESPACE, false))
    {
        this->preferences->remove(key);
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
    if (strlen(ssid) > 0)
    {
        this->setValue(KEY_WIFI_SSID, ssid);
    }
    else
    {
        this->deleteKey(KEY_WIFI_SSID);
    }
}

void Settings::getWIFIPassword(char *password, size_t count)
{
    this->getValue(KEY_WIFI_PASSWORD, password, count);
}

void Settings::setWIFIPassword(const char *password)
{
    if (strlen(password) > 0)
    {
        this->setValue(KEY_WIFI_PASSWORD, password);
    }
    else
    {
        this->deleteKey(KEY_WIFI_PASSWORD);
    }
}

void Settings::getMQTTTelegrafURI(char *uri, size_t count)
{
    this->getValue(KEY_MQTT_TELEGRAF_URI, uri, count);
}

void Settings::setMQTTTelegrafURI(const char *uri)
{
    if (strlen(uri) > 0)
    {
        this->setValue(KEY_MQTT_TELEGRAF_URI, uri);
    }
    else
    {
        this->deleteKey(KEY_MQTT_TELEGRAF_URI);
    }
}

void Settings::getMQTTTelegrafGlobalTopic(char *topic, size_t count)
{
    this->getValue(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC, topic, count);
}

void Settings::setMQTTTelegrafGlobalTopic(const char *topic)
{
    if (strlen(topic) > 0)
    {
        this->setValue(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC, topic);
    }
    else
    {
        this->deleteKey(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC);
    }
}

uint64_t Settings::getTotalMemoryBytes()
{
    return (this->getBigUnsignedIntegerValue(KEY_TOTAL_MEMORY_BYTES, 0));
}

void Settings::setTotalMemoryBytes(uint64_t totalBytes)
{
    if (totalBytes > 0)
    {
        this->setBigIntegerValue(KEY_TOTAL_MEMORY_BYTES, totalBytes);
    }
    else
    {
        this->deleteKey(KEY_TOTAL_MEMORY_BYTES);
    }
}
