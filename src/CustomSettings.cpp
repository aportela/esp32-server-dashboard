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
#define KEY_DEFAULT_SCREEN "DEFAULT_SCREEN"
#define KEY_DASHBOARD_BLOCKS "DBOARD%02u_BLCKS"

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

SCREEN_TYPE CustomSettings::GetDefaultScreen(SCREEN_TYPE defaultScreen)
{
    return ((enum SCREEN_TYPE)this->GetSmallUnsignedIntegerValue(KEY_DEFAULT_SCREEN, (uint8_t)defaultScreen));
}

bool CustomSettings::SetDefaultScreen(SCREEN_TYPE defaultScreen)
{
    if (defaultScreen != SCREEN_TYPE_NONE)
    {
        return (this->SetSmallUnsignedIntegerValue(KEY_DEFAULT_SCREEN, (uint8_t)defaultScreen));
    }
    else
    {
        return (this->DeleteKey(KEY_DEFAULT_SCREEN));
    }
}

bool CustomSettings::getDashboardBlocks(uint8_t index, DASHBOARD_ITEM_TYPE items[DASHBOARD_ITEM_COUNT])
{
#if DASHBOARD_ITEM_COUNT != 5
#error TODO: this method only works with arrays that have same length as DASHBOARD_ITEM_COUNT
#endif
    for (uint8_t i = 0; i < DASHBOARD_ITEM_COUNT; i++)
    {
        items[i] = DASHBOARD_ITEM_TYPE_NONE;
    }
    uint8_t tmpItems[DASHBOARD_ITEM_COUNT] = {0};
    char formattedKey[32] = {'\0'};
    snprintf(formattedKey, sizeof(formattedKey), KEY_DASHBOARD_BLOCKS, index);
    char value[255] = {'\0'};
    this->GetStringValue(formattedKey, value, sizeof(value));
    if (strlen(value) > 0 && sscanf(value, "%u,%u,%u,%u,%u", &tmpItems[0], &tmpItems[1], &tmpItems[2], &tmpItems[3], &tmpItems[4]) == 5)
    {
        for (uint8_t i = 0; i < DASHBOARD_ITEM_COUNT; i++)
        {
            items[i] = (DASHBOARD_ITEM_TYPE)tmpItems[i];
        }
        return (true);
    }
    else
    {
        return (false);
    }
}

bool CustomSettings::setDashboardBlocks(uint8_t index, const DASHBOARD_ITEM_TYPE items[DASHBOARD_ITEM_COUNT])
{
#if DASHBOARD_ITEM_COUNT != 5
#error TODO: this method only works with arrays that have same length as DASHBOARD_ITEM_COUNT
#endif
    char formattedKey[32] = {'\0'};
    if (index > 0 && index < MAX_DASHBOARDS)
    {
        snprintf(formattedKey, sizeof(formattedKey), KEY_DASHBOARD_BLOCKS, index);
        if (items[0] != DASHBOARD_ITEM_TYPE_NONE || items[1] != DASHBOARD_ITEM_TYPE_NONE || items[2] != DASHBOARD_ITEM_TYPE_NONE ||
            items[3] != DASHBOARD_ITEM_TYPE_NONE || items[4] != DASHBOARD_ITEM_TYPE_NONE)
        {
            char value[255] = {'\0'};
            snprintf(value, sizeof(value), "%u,%u,%u,%u,%u", items[0], items[1], items[2], items[3], items[4]);
            return (this->SetStringValue(formattedKey, value));
        }
        else
        {
            return (this->DeleteKey(formattedKey));
        }
    }
    else
    {
        for (uint8_t i = 0; i < DASHBOARD_ITEM_COUNT; i++)
        {
            snprintf(formattedKey, sizeof(formattedKey), KEY_DASHBOARD_BLOCKS, i);
            this->DeleteKey(formattedKey);
        }
        return (true);
    }
}
