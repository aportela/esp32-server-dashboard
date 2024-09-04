#ifndef APORTELA_MICROCONTROLLER_UTILS_MQTT_H
#define APORTELA_MICROCONTROLLER_UTILS_MQTT_H

// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/protocols/mqtt.html
#include <mqtt_client.h>

#define MAX_MQTT_TOPIC_LENGTH 256

namespace aportela::microcontroller::utils
{
    typedef void (*MQTTMessageReceivedCallback)(const char *topic, const char *payload);

    class MQTT
    {
    private:
        static esp_mqtt_client_handle_t client;
        static esp_mqtt_client_config_t mqtt_cfg;
        static MQTTMessageReceivedCallback messageReceivedCallback;
        static char topic[MAX_MQTT_TOPIC_LENGTH];
        static void EventHandler(void *handlerArgs, esp_event_base_t eventBase, int32_t eventId, void *eventData);

    public:
        static void Init(const char *id, const char *uri, const char *topic, const char *username = nullptr, const char *password = nullptr);
        static void Destroy(void);
        static void OnMessageReceived(MQTTMessageReceivedCallback callback);
    };
}

#endif // APORTELA_MICROCONTROLLER_UTILS_MQTT_H
