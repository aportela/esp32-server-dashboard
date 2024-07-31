#include "MQTTTelegrafSource.hpp"
#include "../../utils/MQTT.hpp"
#include <cstring>
#include <inttypes.h>
#include <Arduino.h>

// char MQTTTelegrafSource::cpuTopic[MAX_TOPIC_LENGTH] = {'\0'};
// char MQTTTelegrafSource::memTopic[MAX_TOPIC_LENGTH] = {'\0'};

MQTTTelegrafSource *MQTTTelegrafSource::instance = nullptr;

MQTTTelegrafSource::MQTTTelegrafSource(SourceData *sourceData, const char *uri, const char *clientId, const char *topic) : Source(sourceData)
{
    MQTT::setCallback(MQTTTelegrafSource::onMessageReceived);
    MQTT::init(clientId, uri, topic);

    if (MQTTTelegrafSource::instance == nullptr)
    {
        MQTTTelegrafSource::instance = this;
    }
}

MQTTTelegrafSource::~MQTTTelegrafSource()
{
    if (MQTTTelegrafSource::instance != nullptr)
    {
        MQTTTelegrafSource::instance = nullptr;
    }
}

/*
void MQTTTelegrafSource::setCPUTopic(const char *topic)
{
    if (strlen(topic) <= MAX_TOPIC_LENGTH)
    {
        strncpy(MQTTTelegrafSource::cpuTopic, topic, sizeof(topic) - 1);
        MQTTTelegrafSource::cpuTopic[sizeof(topic) - 1] = '\0';
    }
    else
    {
        // TODO: ERROR
        MQTTTelegrafSource::cpuTopic[0] = '\0';
    }
}

void MQTTTelegrafSource::setMemoryTopic(const char *topic)
{
    if (strlen(topic) <= MAX_TOPIC_LENGTH)
    {
        strncpy(MQTTTelegrafSource::memTopic, topic, sizeof(topic) - 1);
        MQTTTelegrafSource::memTopic[sizeof(topic) - 1] = '\0';
    }
    else
    {
        // TODO: ERROR
        MQTTTelegrafSource::memTopic[0] = '\0';
    }
}
*/

void MQTTTelegrafSource::onMessageReceived(const char *topic, const char *payload)
{
    // Serial.print("Topic: ");
    // Serial.println(topic);
    // Serial.print("Message: ");
    // Serial.println(payload);

    uint64_t currentMessageTimestamp = millis(); // by default use current esp32 timestamp (failover for payload timestamp parse errors)

    // parse timestamp
    size_t payloadLength = strlen(payload);
    const char *lastSpaceSubStr = strrchr(payload, ' ');
    if (lastSpaceSubStr == NULL || lastSpaceSubStr == payload + payloadLength - 1)
    {
    }
    else
    {
        const char *subStr = lastSpaceSubStr + 1;
        uint64_t num = strtoull(subStr, nullptr, 10);
        if (num > 0)
        {
            // set real payload timestamp
            currentMessageTimestamp = num / 1000000;
        }
    }

    if (strcmp(topic, "telegraf/HOST_NAME/cpu") == 0 && strncmp(payload, "cpu,cpu=cpu-total", 17) == 0)
    {

        float usage_idle = 0.0;
        const char *search = "usage_idle=";
        const char *start = strstr(payload, search);
        if (start)
        {
            char subPayload[strlen(payload)];
            strncpy(subPayload, start, sizeof(subPayload) - 1);
            subPayload[sizeof(subPayload) - 1] = '\0';
            const char *format = "usage_idle=%f";
            if (sscanf(subPayload, format, &usage_idle) == 1)
            {
                float cpu_usage = 100.0 - usage_idle;
                MQTTTelegrafSource::instance->sourceData->setCurrentCPULoad(cpu_usage, currentMessageTimestamp);
            }
            else
            {
                Serial.println("Failed to parse payload");
            }
        }
    }
    else if (strcmp(topic, "telegraf/HOST_NAME/mem") == 0)
    {
        uint64_t used = 0;
        const char *search = "used=";
        const char *start = strstr(payload, search);
        if (start)
        {
            char subPayload[strlen(payload)];
            strncpy(subPayload, start, sizeof(subPayload) - 1);
            subPayload[sizeof(subPayload) - 1] = '\0';
            const char *format = "used=%" PRIu64 "i";

            if (sscanf(subPayload, format, &used) == 1)
            {
                MQTTTelegrafSource::instance->sourceData->setUsedMemory(used, currentMessageTimestamp);
            }
            else
            {
                Serial.println("Failed to parse payload");
            }
        }
    }
    else if (strcmp(topic, "telegraf/HOST_NAME/temp") == 0)
    {
        uint64_t celsious = 0;
        const char *search = "temp=";
        const char *start = strstr(payload, search);
        if (start)
        {
            char subPayload[strlen(payload)];
            strncpy(subPayload, start, sizeof(subPayload) - 1);
            subPayload[sizeof(subPayload) - 1] = '\0';
            const char *format = "temp=%f";

            if (sscanf(subPayload, format, &celsious) == 1)
            {
                MQTTTelegrafSource::instance->sourceData->setCurrentCPUTemperature(celsious, currentMessageTimestamp);
            }
            else
            {
                Serial.println("Failed to parse payload");
            }
        }
    }
    else if (strcmp(topic, "telegraf/HOST_NAME/net") == 0 && strncmp(payload, "net,host=HOST_NAME,interface=Wi-Fi", 20) == 0)
    {
        if (strstr(payload, "interface=Wi-Fi"))
        {
            uint64_t recv = 0;
            const char *searchRX = "bytes_recv=";
            const char *startRX = strstr(payload, searchRX);
            if (startRX)
            {
                char subPayload[strlen(payload)];
                strncpy(subPayload, startRX, sizeof(subPayload) - 1);
                subPayload[sizeof(subPayload) - 1] = '\0';
                const char *format = "bytes_recv=%" PRIu64 "i";
                if (sscanf(subPayload, format, &recv) == 1)
                {
                    MQTTTelegrafSource::instance->sourceData->setCurrentTotalNetworkDownloaded(recv, currentMessageTimestamp);
                }
                else
                {
                    Serial.println("Failed to parse RX payload");
                }
            }
            uint64_t sent = 0;
            const char *searchTX = "bytes_sent=";
            const char *startTX = strstr(payload, searchTX);
            if (startTX)
            {
                char subPayload[strlen(payload)];
                strncpy(subPayload, startTX, sizeof(subPayload) - 1);
                subPayload[sizeof(subPayload) - 1] = '\0';
                const char *format = "bytes_sent=%" PRIu64 "i";
                if (sscanf(subPayload, format, &sent) == 1)
                {
                    MQTTTelegrafSource::instance->sourceData->setCurrentTotalNetworkUploaded(sent, currentMessageTimestamp);
                }
                else
                {
                    Serial.println("Failed to parse TX payload");
                }
            }
        }
    }
}
