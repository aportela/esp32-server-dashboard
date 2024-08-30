#include "MQTT.hpp"
#include <cstring>
#include <Arduino.h>

namespace aportela::microcontroller::utils
{
    char MQTT::topic[256] = {'\0'};

    esp_mqtt_client_handle_t MQTT::client = nullptr;
    MQTTMessageReceivedCallback MQTT::messageReceivedCallback = nullptr;

    void MQTT::init(const char *id, const char *uri, const char *topic, const char *username, const char *password)
    {
        esp_mqtt_client_config_t mqtt_cfg = {};
        if (username != nullptr && strlen(username) > 0 && password != nullptr && strlen(password) > 0)
        {
            mqtt_cfg.credentials.username = strdup(username);
            mqtt_cfg.credentials.authentication.password = strdup(password);
        }
        mqtt_cfg.credentials.client_id = strdup(id);
        mqtt_cfg.broker.address.uri = strdup(uri);
        MQTT::client = esp_mqtt_client_init(&mqtt_cfg);
        esp_mqtt_client_register_event(MQTT::client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, MQTT::event_handler, NULL);
        esp_mqtt_client_start(MQTT::client);
        memcpy(MQTT::topic, topic, strlen(topic));
        MQTT::topic[strlen(topic)] = '\0';
    }

    void MQTT::destroy(void)
    {
        if (client != nullptr)
        {
            esp_mqtt_client_stop(MQTT::client);
            esp_mqtt_client_destroy(MQTT::client);
            MQTT::client = nullptr;
        }
        MQTT::messageReceivedCallback = nullptr;
    }

    void MQTT::onMessageReceived(MQTTMessageReceivedCallback callback)
    {
        MQTT::messageReceivedCallback = callback;
    }

    void MQTT::event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
    {
        esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
        esp_mqtt_client_handle_t client = event->client;
        switch ((esp_mqtt_event_id_t)event_id)
        {
        case MQTT_EVENT_CONNECTED:
            esp_mqtt_client_subscribe(client, MQTT::topic, 0);
            break;
        case MQTT_EVENT_DISCONNECTED:
            break;
        case MQTT_EVENT_SUBSCRIBED:
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            break;
        case MQTT_EVENT_PUBLISHED:
            break;
        case MQTT_EVENT_DATA:
            if (MQTT::messageReceivedCallback != nullptr)
            {
                char topic[event->topic_len + 1];
                char payload[event->data_len + 1];
                memcpy(topic, event->topic, event->topic_len);
                topic[event->topic_len] = '\0';
                memcpy(payload, event->data, event->data_len);
                payload[event->data_len] = '\0';
                MQTT::messageReceivedCallback(topic, payload);
            }
            break;
        case MQTT_EVENT_ERROR:
            break;
        default:
            break;
        }
    }
}
