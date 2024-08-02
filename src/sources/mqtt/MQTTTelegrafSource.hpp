#ifndef ESP32_SERVER_DASHBOARD_MQTT_TELEGRAF_SOURCE_H
#define ESP32_SERVER_DASHBOARD_MQTT_TELEGRAF_SOURCE_H

#include <stdint.h>
#include "../Source.hpp"
#include "../../utils/MQTT.hpp"

#define TELEGRAF_MAX_NETWORK_INTERFACE_ID 64

class MQTTTelegrafSource : public Source
{
private:
    static char cpuTopic[MAX_MQTT_TOPIC_LENGTH];
    static char memoryTopic[MAX_MQTT_TOPIC_LENGTH];
    static char temperatureTopic[MAX_MQTT_TOPIC_LENGTH];
    static char systemTopic[MAX_MQTT_TOPIC_LENGTH];
    static char networkTopic[MAX_MQTT_TOPIC_LENGTH];
    static char networkInterfaceId[TELEGRAF_MAX_NETWORK_INTERFACE_ID];
    static MQTTTelegrafSource *instance;
    static bool generateTopic(const char *baseTopic, char *buffer, size_t buffer_size, const char *suffix);
    static void onMessageReceived(const char *topic, const char *payload);

public:
    MQTTTelegrafSource(SourceData *sourceData, const char *uri, const char *clientId, const char *topic, const char *networkInterfaceId);
    ~MQTTTelegrafSource();
};

#endif
