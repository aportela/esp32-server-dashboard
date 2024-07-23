#ifndef ESP32_SERVER_DASHBOARD_MQTT_H
#define ESP32_SERVER_DASHBOARD_MQTT_H

// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/protocols/mqtt.html
#include <mqtt_client.h>

class MQTT
{
private:
    static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

public:
    static void start(void);
};

#endif
