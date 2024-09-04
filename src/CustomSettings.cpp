#include "CustomSettings.hpp"

// WARNING: namespaces with length > 15 WILL NOT OPEN NVS CORRECTLY
#define NAMESPACE "esp32_srv_dashb"

// WARNING: ALSO keys with length > 15 DO NOT WORK
#define KEY_WIFI_SSID "WIFI_SSID"
#define KEY_WIFI_PASSWORD "WIFI_PASSWORD"
#define KEY_MQTT_TELEGRAF_URI "SRC_MQTT_URI"
#define KEY_MQTT_USERNAME "MQTT_USERNAME"
#define KEY_MQTT_PASSWORD "MQTT_PASSWORD"
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

void CustomSettings::GetWIFISSID(char *ssid, size_t count)
{
    this->GetStringValue(KEY_WIFI_SSID, ssid, count);
}

bool CustomSettings::SetWIFISSID(const char *ssid)
{
    if (strlen(ssid) > 0)
    {
        return (this->SetStringValue(KEY_WIFI_SSID, ssid));
    }
    else
    {
        return (this->DeleteKey(KEY_WIFI_SSID));
    }
}

void CustomSettings::GetWIFIPassword(char *password, size_t count)
{
    this->GetStringValue(KEY_WIFI_PASSWORD, password, count);
}

bool CustomSettings::SetWIFIPassword(const char *password)
{
    if (strlen(password) > 0)
    {
        return (this->SetStringValue(KEY_WIFI_PASSWORD, password));
    }
    else
    {
        return (this->DeleteKey(KEY_WIFI_PASSWORD));
    }
}

void CustomSettings::GetMQTTTelegrafURI(char *uri, size_t count)
{
    this->GetStringValue(KEY_MQTT_TELEGRAF_URI, uri, count);
}

bool CustomSettings::SetMQTTTelegrafURI(const char *uri)
{
    if (strlen(uri) > 0)
    {
        return (this->SetStringValue(KEY_MQTT_TELEGRAF_URI, uri));
    }
    else
    {
        return (this->DeleteKey(KEY_MQTT_TELEGRAF_URI));
    }
}

void CustomSettings::GetMQTTUsername(char *username, size_t count)
{
    this->GetStringValue(KEY_MQTT_USERNAME, username, count);
}

bool CustomSettings::SetMQTTUsername(const char *username)
{
    if (strlen(username) > 0)
    {
        return (this->SetStringValue(KEY_MQTT_USERNAME, username));
    }
    else
    {
        return (this->DeleteKey(KEY_MQTT_USERNAME));
    }
}

void CustomSettings::GetMQTTPassword(char *password, size_t count)
{
    this->GetStringValue(KEY_MQTT_PASSWORD, password, count);
}

bool CustomSettings::SetMQTTPassword(const char *password)
{
    if (strlen(password) > 0)
    {
        return (this->SetStringValue(KEY_MQTT_PASSWORD, password));
    }
    else
    {
        return (this->DeleteKey(KEY_MQTT_PASSWORD));
    }
}

void CustomSettings::GetMQTTTelegrafGlobalTopic(char *topic, size_t count)
{
    this->GetStringValue(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC, topic, count);
}

bool CustomSettings::SetMQTTTelegrafGlobalTopic(const char *topic)
{
    if (strlen(topic) > 0)
    {
        return (this->SetStringValue(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC, topic));
    }
    else
    {
        return (this->DeleteKey(KEY_MQTT_TELEGRAF_GLOBAL_TOPIC));
    }
}

uint64_t CustomSettings::GetMaxDownloadBandwidthBytes()
{
    return (this->GetBigUnsignedIntegerValue(KEY_TOTAL_DOWNLOAD_BANDWIDTH_BYTES, 0));
}

bool CustomSettings::SetMaxDownloadBandwidthBytes(uint64_t totalBytes)
{
    if (totalBytes > 0)
    {
        return (this->SetBigUnsigedIntegerValue(KEY_TOTAL_DOWNLOAD_BANDWIDTH_BYTES, totalBytes));
    }
    else
    {
        return (this->DeleteKey(KEY_TOTAL_DOWNLOAD_BANDWIDTH_BYTES));
    }
}

uint64_t CustomSettings::GetMaxUploadBandwidthBytes()
{
    return (this->GetBigUnsignedIntegerValue(KEY_TOTAL_UPLOAD_BANDWIDTH_BYTES, 0));
}

bool CustomSettings::SetMaxUploadBandwidthBytes(uint64_t totalBytes)
{
    if (totalBytes > 0)
    {
        return (this->SetBigUnsigedIntegerValue(KEY_TOTAL_UPLOAD_BANDWIDTH_BYTES, totalBytes));
    }
    else
    {
        return (this->DeleteKey(KEY_TOTAL_UPLOAD_BANDWIDTH_BYTES));
    }
}

void CustomSettings::GetNetworkInterfaceId(char *id, size_t count)
{
    this->GetStringValue(KEY_NET_IFACE_ID, id, count);
}

bool CustomSettings::SetNetworkInterfaceId(const char *id)
{
    if (strlen(id) > 0)
    {
        return (this->SetStringValue(KEY_NET_IFACE_ID, id));
    }
    else
    {
        return (this->DeleteKey(KEY_NET_IFACE_ID));
    }
}

void CustomSettings::GetHostname(char *name, size_t count)
{
    this->GetStringValue(KEY_HOSTNAME, name, count);
}

bool CustomSettings::SetHostname(const char *name)
{
    if (strlen(name) > 0)
    {
        return (this->SetStringValue(KEY_HOSTNAME, name));
    }
    else
    {
        return (this->DeleteKey(KEY_HOSTNAME));
    }
}

bool CustomSettings::GetScreenMirrorFlipVertical()
{
    return (this->GetBoolValue(KEY_SCREEN_MIRROR_FLIP_VERTICAL, false));
}

bool CustomSettings::SetScreenMirrorFlipVertical(bool mirrorFlipVertical)
{
    if (mirrorFlipVertical)
    {
        return (this->SetBoolValue(KEY_SCREEN_MIRROR_FLIP_VERTICAL, mirrorFlipVertical));
    }
    else
    {
        return (this->DeleteKey(KEY_SCREEN_MIRROR_FLIP_VERTICAL));
    }
}

ScreenType CustomSettings::GetDefaultScreen(ScreenType defaultScreen)
{
    return ((enum ScreenType)this->GetSmallUnsignedIntegerValue(DEFAULT_SCREEN, (uint8_t)defaultScreen));
}

bool CustomSettings::SetDefaultScreen(ScreenType defaultScreen)
{
    if (defaultScreen != ST_NONE)
    {
        return (this->SetSmallUnsignedIntegerValue(DEFAULT_SCREEN, (uint8_t)defaultScreen));
    }
    else
    {
        return (this->DeleteKey(DEFAULT_SCREEN));
    }
}
