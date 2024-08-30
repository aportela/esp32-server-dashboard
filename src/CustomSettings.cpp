#include "CustomSettings.hpp"

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
#define KEY_HOSTNAME "HOSTNAME"
#define KEY_SCREEN_MIRROR_FLIP_VERTICAL "SCREEN_MIRROR"
#define DEFAULT_SCREEN "DEFAULT_SCREEN"

CustomSettings::CustomSettings() : Settings(NAMESPACE)
{
}

CustomSettings::~CustomSettings(void)
{
}

void CustomSettings::getWIFISSID(char *ssid, size_t count)
{
    this->getStringValue(KEY_WIFI_SSID, ssid, count);
}

bool CustomSettings::setWIFISSID(const char *ssid)
{
    if (strlen(ssid) > 0)
    {
        return (this->setStringValue(KEY_WIFI_SSID, ssid));
    }
    else
    {
        return (this->deleteKey(KEY_WIFI_SSID));
    }
}

void CustomSettings::getWIFIPassword(char *password, size_t count)
{
    this->getStringValue(KEY_WIFI_PASSWORD, password, count);
}

bool CustomSettings::setWIFIPassword(const char *password)
{
    if (strlen(password) > 0)
    {
        return (this->setStringValue(KEY_WIFI_PASSWORD, password));
    }
    else
    {
        return (this->deleteKey(KEY_WIFI_PASSWORD));
    }
}

void CustomSettings::getMQTTTelegrafURI(char *uri, size_t count)
{
    this->getStringValue(KEY_MQTT_TELEGRAF_URI, uri, count);
}

bool CustomSettings::setMQTTTelegrafURI(const char *uri)
{
    if (strlen(uri) > 0)
    {
        return (this->setStringValue(KEY_MQTT_TELEGRAF_URI, uri));
    }
    else
    {
        return (this->deleteKey(KEY_MQTT_TELEGRAF_URI));
    }
}

void CustomSettings::getMQTTTelegrafGlobalTopic(char *topic, size_t count)
{
    this->getStringValue(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC, topic, count);
}

bool CustomSettings::setMQTTTelegrafGlobalTopic(const char *topic)
{
    if (strlen(topic) > 0)
    {
        return (this->setStringValue(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC, topic));
    }
    else
    {
        return (this->deleteKey(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC));
    }
}

uint64_t CustomSettings::getMaxDownloadBandwidthBytes()
{
    return (this->getBigUnsignedIntegerValue(KEY_TOTAL_DOWNLOAD_BANDWIDTH_BYTES, 0));
}

bool CustomSettings::setMaxDownloadBandwidthBytes(uint64_t totalBytes)
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

uint64_t CustomSettings::getMaxUploadBandwidthBytes()
{
    return (this->getBigUnsignedIntegerValue(KEY_TOTAL_UPLOAD_BANDWIDTH_BYTES, 0));
}

bool CustomSettings::setMaxUploadBandwidthBytes(uint64_t totalBytes)
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

void CustomSettings::getNetworkInterfaceId(char *id, size_t count)
{
    this->getStringValue(KEY_NET_IFACE_ID, id, count);
}

bool CustomSettings::setNetworkInterfaceId(const char *id)
{
    if (strlen(id) > 0)
    {
        return (this->setStringValue(KEY_NET_IFACE_ID, id));
    }
    else
    {
        return (this->deleteKey(KEY_NET_IFACE_ID));
    }
}

void CustomSettings::getHostname(char *name, size_t count)
{
    this->getStringValue(KEY_HOSTNAME, name, count);
}

bool CustomSettings::setHostname(const char *name)
{
    if (strlen(name) > 0)
    {
        return (this->setStringValue(KEY_HOSTNAME, name));
    }
    else
    {
        return (this->deleteKey(KEY_HOSTNAME));
    }
}

bool CustomSettings::getScreenMirrorFlipVertical()
{
    return (this->getBoolValue(KEY_SCREEN_MIRROR_FLIP_VERTICAL, false));
}

bool CustomSettings::setScreenMirrorFlipVertical(bool mirrorFlipVertical)
{
    if (mirrorFlipVertical)
    {
        return (this->setBoolValue(KEY_SCREEN_MIRROR_FLIP_VERTICAL, mirrorFlipVertical));
    }
    else
    {
        return (this->deleteKey(KEY_SCREEN_MIRROR_FLIP_VERTICAL));
    }
}

ScreenType CustomSettings::getDefaultScreen(ScreenType defaultScreen)
{
    return ((enum ScreenType)this->getSmallUnsignedIntegerValue(DEFAULT_SCREEN, (uint8_t)defaultScreen));
}

bool CustomSettings::setDefaultScreen(ScreenType defaultScreen)
{
    if (defaultScreen != ST_NONE)
    {
        return (this->setSmallUnsignedIntegerValue(DEFAULT_SCREEN, (uint8_t)defaultScreen));
    }
    else
    {
        return (this->deleteKey(DEFAULT_SCREEN));
    }
}
