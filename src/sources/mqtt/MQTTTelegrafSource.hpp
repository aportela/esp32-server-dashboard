#ifndef ESP32_SERVER_DASHBOARD_MQTT_TELEGRAF_SOURCE_H
#define ESP32_SERVER_DASHBOARD_MQTT_TELEGRAF_SOURCE_H

#include <stdint.h>
#include "../Source.hpp"
#include "../../utils/MQTT.hpp"

using namespace aportela::microcontroller::utils;

#define WINDOWS

class MQTTTelegrafSource : public Source
{
private:
    static char cpuTopic[MAX_MQTT_TOPIC_LENGTH];
    static char memoryTopic[MAX_MQTT_TOPIC_LENGTH];
    static char sensorsTopic[MAX_MQTT_TOPIC_LENGTH];
    static char temperatureTopic[MAX_MQTT_TOPIC_LENGTH];
    static char systemTopic[MAX_MQTT_TOPIC_LENGTH];
    static char networkTopic[MAX_MQTT_TOPIC_LENGTH];
    static char networkInterfaceId[MAX_NETWORK_INTERFACE_ID_LENGTH];
    static MQTTTelegrafSource *instance;
    static bool generateTopic(const char *baseTopic, char *buffer, size_t bufferSize, const char *suffix);
    static bool getPayloadTokenWithValue(const char *payload, const char *separator, const char *tokenName, char *buffer, size_t bufferSize);
    static void onMessageReceived(const char *topic, const char *payload);

public:
    MQTTTelegrafSource(SourceData *sourceData, const char *uri, const char *clientId, const char *topic, const char *networkInterfaceId, const char *username = nullptr, const char *password = nullptr);
    ~MQTTTelegrafSource();
};

#endif // ESP32_SERVER_DASHBOARD_MQTT_TELEGRAF_SOURCE_H
