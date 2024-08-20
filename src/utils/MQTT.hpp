#ifndef ESP32_SERVER_DASHBOARD_MQTT_H
#define ESP32_SERVER_DASHBOARD_MQTT_H

// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/protocols/mqtt.html
#include <mqtt_client.h>

#define MAX_MQTT_TOPIC_LENGTH 256

typedef void (*MQTTMessageReceivedCallback)(const char *topic, const char *payload);

class MQTT
{
private:
    static esp_mqtt_client_handle_t client;
    static MQTTMessageReceivedCallback messageReceivedCallback;
    static void event_handler(void *handler_args, esp_event_base_t event_base, int32_t event_id, void *event_data);
    static char topic[MAX_MQTT_TOPIC_LENGTH];

public:
    static void init(const char *id, const char *uri, const char *topic);
    static void destroy(void);
    static void onMessageReceived(MQTTMessageReceivedCallback callback);
};

#endif
