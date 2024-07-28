#ifndef ESP32_SERVER_DASHBOARD_MQTT_TELEGRAF_SOURCE_H
#define ESP32_SERVER_DASHBOARD_MQTT_TELEGRAF_SOURCE_H

#include <stdint.h>
#include "../Source.hpp"

#define MAX_TOPIC_LENGTH 128

class MQTTTelegrafSource : public Source
{
private:
    // static char cpuTopic[MAX_TOPIC_LENGTH];
    // static char memTopic[MAX_TOPIC_LENGTH];
    static MQTTTelegrafSource *instance;
    static void onMessageReceived(const char *topic, const char *payload);

public:
    MQTTTelegrafSource(const char *uri, const char *clientId, const char *topic);
    ~MQTTTelegrafSource();
    // static void setCPUTopic(const char *topic);
    // static void setMemoryTopic(const char *topic);
};

#endif
