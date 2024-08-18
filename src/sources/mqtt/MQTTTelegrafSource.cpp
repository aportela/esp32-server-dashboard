#include "MQTTTelegrafSource.hpp"
#include <cstring>
#include <inttypes.h>
#include <Arduino.h>

// #define DEBUG_MQTT_TELEGRAF

char MQTTTelegrafSource::cpuTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::memoryTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::temperatureTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::systemTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::networkTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::networkInterfaceId[MAX_NETWORK_INTERFACE_ID_LENGTH] = {'\0'};

MQTTTelegrafSource *MQTTTelegrafSource::instance = nullptr;

MQTTTelegrafSource::MQTTTelegrafSource(SourceData *sourceData, const char *uri, const char *clientId, const char *topic) : Source(sourceData)
{
#ifdef DEBUG_MQTT_TELEGRAF
    Serial.println("MQTTTelegrafSource");
    Serial.print("clientId: ");
    Serial.println(clientId);
    Serial.print("uri: ");
    Serial.println(uri);
    Serial.println("topic: ");
    Serial.println(topic);
#endif
    MQTT::setCallback(MQTTTelegrafSource::onMessageReceived);
    MQTT::init(clientId, uri, topic);
    MQTTTelegrafSource::generateTopic(topic, MQTTTelegrafSource::cpuTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/cpu");
    MQTTTelegrafSource::generateTopic(topic, MQTTTelegrafSource::memoryTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/mem");
    MQTTTelegrafSource::generateTopic(topic, MQTTTelegrafSource::temperatureTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/temp");
    MQTTTelegrafSource::generateTopic(topic, MQTTTelegrafSource::systemTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/system");
    MQTTTelegrafSource::generateTopic(topic, MQTTTelegrafSource::networkTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/net");

    sourceData->getNetworkInterfaceId(MQTTTelegrafSource::networkInterfaceId, sizeof(MQTTTelegrafSource::networkInterfaceId));

#ifdef DEBUG_MQTT_TELEGRAF
    Serial.println("networkInterfaceId: ");
    Serial.println(MQTTTelegrafSource::networkInterfaceId);
#endif
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

// generate "sub-topic" from base topic (replace end "/#"" on base topic with suffix)
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
#ifdef DEBUG_MQTT_TELEGRAF
    Serial.print("Topic:");
    Serial.println(topic);
    Serial.print("Message:");
    Serial.println(payload);
#endif
    uint64_t currentMessageTimestamp = millis(); // by default use current esp32 timestamp (failover for payload timestamp parse errors)
    // parse timestamp
    size_t payloadLength = strlen(payload);
    const char *lastSpaceSubStr = strrchr(payload, ' ');
    if (lastSpaceSubStr == NULL || lastSpaceSubStr == payload + payloadLength - 1)
    {
#ifdef DEBUG_MQTT_TELEGRAF
        Serial.println("Error parsing payload timestamp");
#endif
    }
    else
    {
        const char *subStr = lastSpaceSubStr + 1;
        uint64_t payloadTimestamp = strtoull(subStr, nullptr, 10);
        if (payloadTimestamp > 0)
        {
            // set real payload timestamp
            currentMessageTimestamp = payloadTimestamp / 1000000;
        }
        else
        {
#ifdef DEBUG_MQTT_TELEGRAF
            Serial.println("Error parsing payload timestamp");
#endif
        }
    }

    if (strcmp(topic, MQTTTelegrafSource::cpuTopic) == 0 && strncmp(payload, "cpu,cpu=cpu-total", 17) == 0)
    {

        const char *payloadSubStr = strstr(payload, "usage_idle=");
        if (payloadSubStr)
        {
            float usageIdle = 0.0;
            if (sscanf(payloadSubStr, "usage_idle=%f", &usageIdle) == 1)
            {
                float cpu_usage = usageIdle < 100 ? 100.0f - usageIdle : 100;
                MQTTTelegrafSource::instance->sourceData->setCurrentCPULoad(cpu_usage, currentMessageTimestamp);
            }
            else
            {
#ifdef DEBUG_MQTT_TELEGRAF
                Serial.println("Error parsing CPU usage idle value");
#endif
            }
        }
    }
    else if (strcmp(topic, MQTTTelegrafSource::memoryTopic) == 0)
    {
        const char *payloadSubStr = strstr(payload, "used=");
        if (payloadSubStr)
        {
            uint64_t usedBytes = 0;
            if (sscanf(payloadSubStr, "used=%" PRIu64 "i", &usedBytes) == 1)
            {
                uint64_t totalBytes = 0;
                const char *totalSubStrStart = strstr(payload, "total=");
                if (sscanf(totalSubStrStart, "total=%" PRIu64 "i", &totalBytes) == 1)
                {
                    MQTTTelegrafSource::instance->sourceData->setCurrentUsedMemory(usedBytes, totalBytes, currentMessageTimestamp);
                }
                else
                {
#ifdef DEBUG_MQTT_TELEGRAF
                    Serial.println("Error parsing MEM total value");
#endif
                }
            }
            else
            {
#ifdef DEBUG_MQTT_TELEGRAF
                Serial.println("Error parsing MEM used value");
#endif
            }
        }
    }
    else if (strcmp(topic, MQTTTelegrafSource::temperatureTopic) == 0)
    {
        const char *payloadSubStr = strstr(payload, "temp=");
        if (payloadSubStr)
        {
            float temp = 0;
            if (sscanf(payloadSubStr, "temp=%f", &temp) == 1)
            {
                MQTTTelegrafSource::instance->sourceData->setCurrentCPUTemperature(temp, currentMessageTimestamp);
            }
            else
            {
#ifdef DEBUG_MQTT_TELEGRAF
                Serial.println("Error parsing temperature value");
#endif
            }
        }
    }
    else if (strcmp(topic, MQTTTelegrafSource::systemTopic) == 0)
    {
        const char *payloadSubStr = strstr(payload, "uptime=");
        if (payloadSubStr)
        {
            uint64_t uptime = 0;
            // WARNING: (BEFORE REPORTING BUG) WINDOWS REPORTS "INVALID" UPTIME IF YOU ARE USING "FAST STARTUP" SO IF YOUR SERVER UPTIME SEEMS TO BE WRONG CHECK IF YOU HAVE THIS FEATURE ENABLED
            // https://answers.microsoft.com/en-us/windows/forum/all/fast-startup-doesnt-break-power-cycle/07bd6bf8-dd24-4c77-b911-17df89c74eb3
            if (sscanf(payloadSubStr, "uptime=%" PRIu64 "i", &uptime) == 1)
            {
                MQTTTelegrafSource::instance->sourceData->setCurrentUptime(uptime, currentMessageTimestamp);
            }
            else
            {
#ifdef DEBUG_MQTT_TELEGRAF
                Serial.println("Error parsing uptime value");
#endif
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
            const char *payloadBytesRecvSubStr = strstr(payload, "bytes_recv=");
            if (payloadBytesRecvSubStr)
            {
                uint64_t bytesRecv = 0;
                if (sscanf(payloadBytesRecvSubStr, "bytes_recv=%" PRIu64 "i", &bytesRecv) == 1)
                {
                    MQTTTelegrafSource::instance->sourceData->setCurrentNetworkDownload(bytesRecv, currentMessageTimestamp);
                }
                else
                {
#ifdef DEBUG_MQTT_TELEGRAF
                    Serial.println("Error parsing network received bytes value");
#endif
                }
            }
            const char *payloadBytesSentSubStr = strstr(payload, "bytes_sent=");
            if (payloadBytesSentSubStr)
            {
                uint64_t bytesSent = 0;
                if (sscanf(payloadBytesSentSubStr, "bytes_sent=%" PRIu64 "i", &bytesSent) == 1)
                {
                    MQTTTelegrafSource::instance->sourceData->setCurrentNetworkUpload(bytesSent, currentMessageTimestamp);
                }
                else
                {
#ifdef DEBUG_MQTT_TELEGRAF
                    Serial.println("Error parsing network sent bytes value");
#endif
                }
            }
        }
    }
}
