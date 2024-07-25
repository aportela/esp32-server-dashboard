#ifndef ESP32_SERVER_DASHBOARD_MQTT_TELEGRAF_SOURCE_H
#define ESP32_SERVER_DASHBOARD_MQTT_TELEGRAF_SOURCE_H

#define MIN_CPU_LOAD 0
#define MAX_CPU_LOAD 100

#define MIN_MEMORY 0
#define MAX_MEMORY 34231410688 // 32 GB

#define MIN_CPU_TEMPERATURE 0
#define MAX_CPU_TEMPERATURE 100 // Celsious

#define MIN_NETWORK_DOWNLOAD_BANDWITH 0
#define MAX_NETWORK_DOWNLOAD_BANDWITH 536870912 // Mbytes

#define MIN_NETWORK_UPLOAD_BANDWITH 0
#define MAX_NETWORK_UPLOAD_BANDWITH 536870912 // Mbytes

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
    uint64_t getCurrentCPULoad(void) override;
    uint64_t getTotalMemory(void) override;
    uint64_t getUsedMemory(void) override;
    uint64_t getCurrentCPUTemperature(void) override;
    uint64_t getTotalNetworkDownloadBandwith(void) override;
    uint64_t getUsedNetworkDownloadBandwith(void) override;
    uint64_t getTotalNetworkUploadBandwith(void) override;
    uint64_t getUsedNetworkUploadBandwith(void) override;
};

#endif
