#include "MQTTTelegrafSource.hpp"
#include <cstring>
#include <inttypes.h>
#include <Arduino.h>

char MQTTTelegrafSource::cpuTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::memoryTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::temperatureTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::systemTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::networkTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::networkInterfaceId[MAX_NETWORK_INTERFACE_ID_LENGTH] = {'\0'};

MQTTTelegrafSource *MQTTTelegrafSource::instance = nullptr;

MQTTTelegrafSource::MQTTTelegrafSource(SourceData *sourceData, const char *uri, const char *clientId, const char *topic) : Source(sourceData)
{
    MQTT::setCallback(MQTTTelegrafSource::onMessageReceived);
    MQTT::init(clientId, uri, topic);

    MQTTTelegrafSource::generateTopic(topic, MQTTTelegrafSource::cpuTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/cpu");
    MQTTTelegrafSource::generateTopic(topic, MQTTTelegrafSource::memoryTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/mem");
    MQTTTelegrafSource::generateTopic(topic, MQTTTelegrafSource::temperatureTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/temp");
    MQTTTelegrafSource::generateTopic(topic, MQTTTelegrafSource::systemTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/system");
    MQTTTelegrafSource::generateTopic(topic, MQTTTelegrafSource::networkTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/net");

    sourceData->getNetworkInterfaceId(MQTTTelegrafSource::networkInterfaceId, sizeof(MQTTTelegrafSource::networkInterfaceId));

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

bool MQTTTelegrafSource::generateTopic(const char *baseTopic, char *buffer, size_t buffer_size, const char *suffix)
{
    size_t baseTopicLength = strlen(baseTopic);
    size_t suffixLength = strlen(suffix);

    if (baseTopicLength > 2 && baseTopic[baseTopicLength - 2] == '/' && baseTopic[baseTopicLength - 1] == '#')
    {
        if (buffer_size > baseTopicLength - 2 + suffixLength + 1)
        {
            strncpy(buffer, baseTopic, baseTopicLength - 2);
            buffer[baseTopicLength - 2] = '\0';
            strcat(buffer, suffix);
            return (true);
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}

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

    if (strcmp(topic, MQTTTelegrafSource::cpuTopic) == 0 && strncmp(payload, "cpu,cpu=cpu-total", 17) == 0)
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
                float cpu_usage = usage_idle < 100 ? 100.0f - usage_idle : 100;
                MQTTTelegrafSource::instance->sourceData->setCurrentCPULoad(cpu_usage, currentMessageTimestamp);
            }
            else
            {
                Serial.println("Failed to parse cpu payload");
            }
        }
    }
    else if (strcmp(topic, MQTTTelegrafSource::memoryTopic) == 0)
    {
        const char *usedSubStrStart = strstr(payload, "used=");
        if (usedSubStrStart)
        {
            uint64_t usedBytes = 0;
            if (sscanf(usedSubStrStart, "used=%" PRIu64 "i", &usedBytes) == 1)
            {
                uint64_t totalBytes = 0;
                const char *totalSubStrStart = strstr(payload, "total=");
                if (sscanf(totalSubStrStart, "total=%" PRIu64 "i", &totalBytes) == 1)
                {
                    MQTTTelegrafSource::instance->sourceData->setUsedAndTotalMemory(usedBytes, totalBytes, currentMessageTimestamp);
                }
                else
                {
                    MQTTTelegrafSource::instance->sourceData->setUsedMemory(usedBytes, currentMessageTimestamp);
                }
            }
            else
            {
                Serial.println("Failed to parse mem payload");
            }
        }
    }
    else if (strcmp(topic, MQTTTelegrafSource::temperatureTopic) == 0)
    {
        float celsious = 0;
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
                Serial.println("Failed to parse temp payload");
            }
        }
    }
    else if (strcmp(topic, MQTTTelegrafSource::systemTopic) == 0)
    {
        uint64_t uptimeSeconds = 0;
        const char *search = "uptime=";
        const char *start = strstr(payload, search);
        if (start)
        {
            char subPayload[strlen(payload)];
            strncpy(subPayload, start, sizeof(subPayload) - 1);
            subPayload[sizeof(subPayload) - 1] = '\0';
            // WARNING: (BEFORE REPORTING BUG) WINDOWS REPORTS "INVALID" UPTIME IF YOU ARE USING "FAST STARTUP" SO IF YOUR SERVER UPTIME SEEMS TO BE WRONG CHECK IF YOU HAVE THIS FEATURE ENABLED
            // https://answers.microsoft.com/en-us/windows/forum/all/fast-startup-doesnt-break-power-cycle/07bd6bf8-dd24-4c77-b911-17df89c74eb3
            const char *format = "uptime=%" PRIu64 "i";
            if (sscanf(subPayload, format, &uptimeSeconds) == 1)
            {
                MQTTTelegrafSource::instance->sourceData->setCurrentUptimeSeconds(uptimeSeconds, currentMessageTimestamp);
            }
            else
            {
                Serial.println("Failed to parse system payload");
            }
        }
    }
    else if (strcmp(topic, MQTTTelegrafSource::networkTopic) == 0 && MQTTTelegrafSource::networkInterfaceId != NULL && strlen(MQTTTelegrafSource::networkInterfaceId) > 0)
    {
        // search for interface id (telegraf can send data for multiple network interfaces)
        char payloadSearchStr[10 + MAX_NETWORK_INTERFACE_ID_LENGTH] = {'\0'};
        snprintf(payloadSearchStr, sizeof(payloadSearchStr), "interface=%s", MQTTTelegrafSource::networkInterfaceId);
        if (strstr(payload, payloadSearchStr))
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
                    Serial.println("Failed to parse net RX payload");
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
                    Serial.println("Failed to parse net TX payload");
                }
            }
        }
    }
}
