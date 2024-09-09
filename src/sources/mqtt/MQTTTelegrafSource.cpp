#include "MQTTTelegrafSource.hpp"
#include <cstring>
#include <cstdio>
#include <inttypes.h>
#include <Arduino.h>

// #define DEBUG_MQTT_TELEGRAF

char MQTTTelegrafSource::cpuTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::memoryTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::sensorsTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::temperatureTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::systemTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::networkTopic[MAX_MQTT_TOPIC_LENGTH] = {'\0'};
char MQTTTelegrafSource::networkInterfaceId[MAX_NETWORK_INTERFACE_ID_LENGTH] = {'\0'};

MQTTTelegrafSource *MQTTTelegrafSource::instance = nullptr;

MQTTTelegrafSource::MQTTTelegrafSource(SourceData *sourceData, const char *uri, const char *clientId, const char *topic, const char *networkInterfaceId, const char *username, const char *password) : Source(sourceData)
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
    MQTT::OnMessageReceived(MQTTTelegrafSource::OnMessageReceived);
    MQTT::Init(clientId, uri, topic, username, password);
    MQTTTelegrafSource::GenerateTopic(topic, MQTTTelegrafSource::cpuTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/cpu");
    MQTTTelegrafSource::GenerateTopic(topic, MQTTTelegrafSource::memoryTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/mem");
    MQTTTelegrafSource::GenerateTopic(topic, MQTTTelegrafSource::sensorsTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/sensors");
    MQTTTelegrafSource::GenerateTopic(topic, MQTTTelegrafSource::temperatureTopic, sizeof(MQTTTelegrafSource::temperatureTopic), "/temp");
    MQTTTelegrafSource::GenerateTopic(topic, MQTTTelegrafSource::systemTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/system");
    MQTTTelegrafSource::GenerateTopic(topic, MQTTTelegrafSource::networkTopic, sizeof(MQTTTelegrafSource::cpuTopic), "/net");

    if (strlen(networkInterfaceId) > 0)
    {
        strncpy(MQTTTelegrafSource::networkInterfaceId, networkInterfaceId, sizeof(MQTTTelegrafSource::networkInterfaceId));
    }

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
        MQTT::Destroy();
        MQTTTelegrafSource::instance = nullptr;
    }
}

// generate "sub-topic" from base topic (replace end "/#"" on base topic with suffix)
bool MQTTTelegrafSource::GenerateTopic(const char *baseTopic, char *buffer, size_t bufferSize, const char *suffix)
{
    size_t baseTopicLength = strlen(baseTopic);
    size_t suffixLength = strlen(suffix);

    if (baseTopicLength > 2 && baseTopic[baseTopicLength - 2] == '/' && baseTopic[baseTopicLength - 1] == '#')
    {
        if (bufferSize > baseTopicLength - 2 + suffixLength + 1)
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

bool MQTTTelegrafSource::GetPayloadTokenWithValue(const char *payload, const char *separator, const char *tokenName, char *buffer, size_t bufferSize)
{
    char *newStr = strdup(payload);
    if (newStr == NULL)
    {
        return (false);
    }
    char pattern[strlen(tokenName) + 3];
    std::snprintf(pattern, sizeof(pattern), "%s=", tokenName);
    char *token = strtok(newStr, separator);
    while (token != NULL)
    {
        if (strncmp(token, pattern, strlen(pattern)) == 0)
        {
            strncpy(buffer, token, bufferSize);
            free(newStr);
            return (true);
        }
        else
        {
            token = strtok(NULL, separator);
        }
    }
    free(newStr);
    return (false);
}

void MQTTTelegrafSource::OnMessageReceived(const char *topic, const char *payload)
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

    // remove start payload unrequired data (input type, host)
    char *cleanPayloadPtr = strchr(payload, ' ');
    if (cleanPayloadPtr != NULL)
    {
        cleanPayloadPtr = cleanPayloadPtr + 1;
    }
    char tokenWithValue[255] = {'\0'};
    if (strcmp(topic, MQTTTelegrafSource::cpuTopic) == 0 && strncmp(payload, "cpu,cpu=cpu-total", 17) == 0)
    {
        float usageSystem = 0.0f;
        float usageUser = 0.0f;
        float usageIdle = 0.0f;
        float usageNice = 0.0f;
        float usageIOWait = 0.0f;
        float usageIRQ = 0.0f;
        float usageSoftIRQ = 0.0f;
        float usageGuest = 0.0f;
        float usageGuestNice = 0.0f;
        float usageSteal = 0.0f;
        char *tmpPayload = strdup(cleanPayloadPtr);
        uint8_t totalValidTokens = 0;
        if (tmpPayload != NULL)
        {
            char *token = strtok(tmpPayload, ",");
            while (token != NULL)
            {
                if (strncmp(token, "usage_system=", strlen("usage_system=")) == 0)
                {
                    if (sscanf(token, "usage_system=%f", &usageSystem) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing CPU usage system value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "usage_user=", strlen("usage_user=")) == 0)
                {
                    if (sscanf(token, "usage_user=%f", &usageUser) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing CPU usage user value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "usage_idle=", strlen("usage_idle=")) == 0)
                {
                    if (sscanf(token, "usage_idle=%f", &usageIdle) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing CPU usage idle value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "usage_nice=", strlen("usage_nice=")) == 0)
                {
                    if (sscanf(token, "usage_nice=%f", &usageNice) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing CPU usage nice value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "usage_iowait=", strlen("usage_iowait=")) == 0)
                {
                    if (sscanf(token, "usage_iowait=%f", &usageIOWait) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing CPU usage iowait value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "usage_irq=", strlen("usage_irq=")) == 0)
                {
                    if (sscanf(token, "usage_irq=%f", &usageIRQ) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing CPU usage irq value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "usage_softirq=", strlen("usage_softirq=")) == 0)
                {
                    if (sscanf(token, "usage_softirq=%f", &usageSoftIRQ) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing CPU usage soft irq value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "usage_guest=", strlen("usage_guest=")) == 0)
                {
                    if (sscanf(token, "usage_guest=%f", &usageGuest) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing CPU usage guest value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "usage_guest_nice=", strlen("usage_guest_nice=")) == 0)
                {
                    if (sscanf(token, "usage_guest_nice=%f", &usageGuestNice) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing CPU usage guest nice value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "usage_steal=", strlen("usage_steal=")) == 0)
                {
                    if (sscanf(token, "usage_steal=%f", &usageSteal) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing CPU usage steal value (read error)");
#endif
                    }
                }
                token = strtok(NULL, ",");
            }
            free(tmpPayload);
        }
        if (totalValidTokens == 10)
        {
            MQTTTelegrafSource::instance->sourceData->SetCurrentCPUData(
                usageIdle <= 100 ? 100.0f - usageIdle : 0,
                usageSystem,
                usageUser,
                usageIdle,
                usageNice,
                usageIOWait,
                usageIRQ,
                usageSoftIRQ,
                usageGuest,
                usageGuestNice,
                usageSteal,
                currentMessageTimestamp);
        }
        else
        {
#ifdef DEBUG_MQTT_TELEGRAF
            Serial.printf("Error parsing CPU usage values (tokens found: %d of %d)\n", totalValidTokens, 10);
#endif
        }
    }
    else if (strcmp(topic, MQTTTelegrafSource::memoryTopic) == 0)
    {
        SourceDataQueueUsedMemoryValues currentData;
        currentData.timestamp = currentMessageTimestamp;
        uint8_t totalValidTokens = 0;
        char *tmpPayload = strdup(cleanPayloadPtr);
        if (tmpPayload != NULL)
        {
            char *token = strtok(tmpPayload, ",");
            while (token != NULL)
            {
                if (strncmp(token, "active=", strlen("active=")) == 0)
                {
                    if (sscanf(token, "active=%" PRIu64, &currentData.active) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage active value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "available=", strlen("available=")) == 0)
                {
                    if (sscanf(token, "available=%" PRIu64, &currentData.available) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage available value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "available_percent=", strlen("available_percent=")) == 0)
                {
                    if (sscanf(token, "available_percent=%f", &currentData.availablePercent) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage available percent value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "buffered=", strlen("buffered=")) == 0)
                {
                    if (sscanf(token, "buffered=%" PRIu64, &currentData.buffered) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage buffered value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "cached=", strlen("cached=")) == 0)
                {
                    if (sscanf(token, "cached=%" PRIu64, &currentData.cached) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage cached value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "commit_limit=", strlen("commit_limit=")) == 0)
                {
                    if (sscanf(token, "commit_limit=%" PRIu64, &currentData.commitLimit) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage commit limit value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "committed_as=", strlen("committed_as=")) == 0)
                {
                    if (sscanf(token, "committed_as=%" PRIu64, &currentData.committedAs) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage committed as value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "dirty=", strlen("dirty=")) == 0)
                {
                    if (sscanf(token, "dirty=%" PRIu64, &currentData.dirty) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage dirty value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "free=", strlen("free=")) == 0)
                {
                    if (sscanf(token, "free=%" PRIu64, &currentData.free) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage free value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "high_free=", strlen("high_free=")) == 0)
                {
                    if (sscanf(token, "high_free=%" PRIu64, &currentData.highFree) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage high free value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "high_total=", strlen("high_total=")) == 0)
                {
                    if (sscanf(token, "high_total=%" PRIu64, &currentData.highTotal) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage high total value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "huge_pages_free=", strlen("huge_pages_free=")) == 0)
                {
                    if (sscanf(token, "huge_pages_free=%" PRIu64, &currentData.hugePagesFree) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage huge pages free value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "huge_page_size=", strlen("huge_page_size=")) == 0)
                {
                    if (sscanf(token, "huge_page_size=%" PRIu64, &currentData.hugePageSize) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage huge page size value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "huge_pages_total=", strlen("huge_pages_total=")) == 0)
                {
                    if (sscanf(token, "huge_pages_total=%" PRIu64, &currentData.hugePagesTotal) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage huge pages total value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "inactive=", strlen("inactive=")) == 0)
                {
                    if (sscanf(token, "inactive=%" PRIu64, &currentData.inactive) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage inactive value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "laundry=", strlen("laundry=")) == 0)
                {
                    if (sscanf(token, "laundry=%" PRIu64, &currentData.laundry) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage laundry value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "low_free=", strlen("low_free=")) == 0)
                {
                    if (sscanf(token, "low_free=%" PRIu64, &currentData.lowFree) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage low free value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "low_total=", strlen("low_total=")) == 0)
                {
                    if (sscanf(token, "low_total=%" PRIu64, &currentData.lowTotal) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage low total value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "mapped=", strlen("mapped=")) == 0)
                {
                    if (sscanf(token, "mapped=%" PRIu64, &currentData.mapped) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage mapped value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "page_tables=", strlen("page_tables=")) == 0)
                {
                    if (sscanf(token, "page_tables=%" PRIu64, &currentData.pageTables) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage page tables value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "shared=", strlen("shared=")) == 0)
                {
                    if (sscanf(token, "shared=%" PRIu64, &currentData.shared) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage shared value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "slab=", strlen("slab=")) == 0)
                {
                    if (sscanf(token, "slab=%" PRIu64, &currentData.slab) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage slab value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "sreclaimable=", strlen("sreclaimable=")) == 0)
                {
                    if (sscanf(token, "sreclaimable=%" PRIu64, &currentData.sreclaimable) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage sreclaimable value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "sunreclaim=", strlen("sunreclaim=")) == 0)
                {
                    if (sscanf(token, "sunreclaim=%" PRIu64, &currentData.sunreclaim) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage sunreclaim value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "swap_cached=", strlen("swap_cached=")) == 0)
                {
                    if (sscanf(token, "swap_cached=%" PRIu64, &currentData.swapCached) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage swap cached value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "swap_free=", strlen("swap_free=")) == 0)
                {
                    if (sscanf(token, "swap_free=%" PRIu64, &currentData.swapFree) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage swap free value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "swap_total=", strlen("swap_total=")) == 0)
                {
                    if (sscanf(token, "swap_total=%" PRIu64, &currentData.swapTotal) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage swap total value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "total=", strlen("total=")) == 0)
                {
                    if (sscanf(token, "total=%" PRIu64, &currentData.total) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage total value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "used=", strlen("used=")) == 0)
                {
                    if (sscanf(token, "used=%" PRIu64, &currentData.used) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage used value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "used_percent=", strlen("used_percent=")) == 0)
                {
                    if (sscanf(token, "used_percent=%f", &currentData.usedPercent) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage used percent value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "vmalloc_chunk=", strlen("vmalloc_chunk=")) == 0)
                {
                    if (sscanf(token, "vmalloc_chunk=%" PRIu64, &currentData.vmallocChunk) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage vmalloc chunk value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "vmalloc_total=", strlen("vmalloc_total=")) == 0)
                {
                    if (sscanf(token, "vmalloc_total=%" PRIu64, &currentData.vmallocTotal) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage vmalloc total value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "vmalloc_used=", strlen("vmalloc_used=")) == 0)
                {
                    if (sscanf(token, "vmalloc_used=%" PRIu64, &currentData.vmallocUsed) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage vmalloc used value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "wired=", strlen("wired=")) == 0)
                {
                    if (sscanf(token, "wired=%" PRIu64, &currentData.wired) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage wired value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "write_back=", strlen("write_back=")) == 0)
                {
                    if (sscanf(token, "write_back=%" PRIu64, &currentData.writeBack) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage write back (read error)");
#endif
                    }
                }
                else if (strncmp(token, "write_back_tmp=", strlen("write_back_tmp=")) == 0)
                {
                    if (sscanf(token, "write_back_tmp=%" PRIu64, &currentData.writeBackTmp) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing memory usage write back tmp (read error)");
#endif
                    }
                }
                token = strtok(NULL, ",");
            }
            free(tmpPayload);
        }
        if (totalValidTokens > 0)
        {
            MQTTTelegrafSource::instance->sourceData->SetCurrentMemoryData(currentData);
        }
        else
        {
#ifdef DEBUG_MQTT_TELEGRAF
            Serial.printf("Error parsing memory usage values (tokens found: %d of %d)\n", totalValidTokens, 10);
#endif
        }
    }
    else if (strcmp(topic, MQTTTelegrafSource::sensorsTopic) == 0)
    {
        if (MQTTTelegrafSource::GetPayloadTokenWithValue(payload, ",", "feature", tokenWithValue, sizeof(tokenWithValue)) && strncmp(tokenWithValue, "feature=package_id_0", 20) == 0)
        {
            if (MQTTTelegrafSource::GetPayloadTokenWithValue(cleanPayloadPtr, ",", "temp_input", tokenWithValue, sizeof(tokenWithValue)))
            {
                float temp = 0;
                if (sscanf(tokenWithValue, "temp_input=%f", &temp) == 1)
                {
                    MQTTTelegrafSource::instance->sourceData->SetCurrentCPUTemperature(temp, currentMessageTimestamp);
                }
                else
                {
#ifdef DEBUG_MQTT_TELEGRAF
                    Serial.println("Error parsing CPU temperature value (read error)");
#endif
                }
            }
            else
            {
#ifdef DEBUG_MQTT_TELEGRAF
                Serial.println("Error parsing CPU temperature value (token not found)");
#endif
            }
        }
        else
        {
#ifdef DEBUG_MQTT_TELEGRAF
            Serial.println("Error parsing CPU temperature value (token not found)");
#endif
        }
    }
    else if (strcmp(topic, MQTTTelegrafSource::temperatureTopic) == 0)
    {
        if (MQTTTelegrafSource::GetPayloadTokenWithValue(cleanPayloadPtr, ",", "temp", tokenWithValue, sizeof(tokenWithValue)))
        {
            float temp = 0;
            if (sscanf(tokenWithValue, "temp=%f", &temp) == 1)
            {
                MQTTTelegrafSource::instance->sourceData->SetCurrentCPUTemperature(temp, currentMessageTimestamp);
            }
            else
            {
#ifdef DEBUG_MQTT_TELEGRAF
                Serial.println("Error parsing CPU temperature value (read error)");
#endif
            }
        }
        else
        {
#ifdef DEBUG_MQTT_TELEGRAF
            Serial.println("Error parsing CPU temperature value (token not found)");
#endif
        }
    }
    else if (strcmp(topic, MQTTTelegrafSource::systemTopic) == 0)
    {
        if (MQTTTelegrafSource::GetPayloadTokenWithValue(cleanPayloadPtr, ",", "uptime", tokenWithValue, sizeof(tokenWithValue)))
        {
            uint64_t uptime = 0;
            // WARNING: (BEFORE REPORTING BUG) WINDOWS REPORTS "INVALID" UPTIME IF YOU ARE USING "FAST STARTUP" SO IF YOUR SERVER UPTIME SEEMS TO BE WRONG CHECK IF YOU HAVE THIS FEATURE ENABLED
            // https://answers.microsoft.com/en-us/windows/forum/all/fast-startup-doesnt-break-power-cycle/07bd6bf8-dd24-4c77-b911-17df89c74eb3
            if (sscanf(tokenWithValue, "uptime=%" PRIu64, &uptime) == 1)
            {
                MQTTTelegrafSource::instance->sourceData->SetCurrentUptime(uptime, currentMessageTimestamp);
            }
            else
            {
#ifdef DEBUG_MQTT_TELEGRAF
                Serial.println("Error parsing uptime value (read error)");
#endif
            }
        }
        else
        {
#ifdef DEBUG_MQTT_TELEGRAF
            Serial.println("Error parsing uptime value (token not found)");
#endif
        }
    }
    else if (strcmp(topic, MQTTTelegrafSource::networkTopic) == 0 && MQTTTelegrafSource::networkInterfaceId != NULL && strlen(MQTTTelegrafSource::networkInterfaceId) > 0)
    {
        // search for interface id (telegraf can send data for multiple network interfaces)
        char payloadSearchStr[10 + MAX_NETWORK_INTERFACE_ID_LENGTH] = {'\0'};
        std::snprintf(payloadSearchStr, sizeof(payloadSearchStr), "interface=%s", MQTTTelegrafSource::networkInterfaceId);
        if (strstr(payload, payloadSearchStr))
        {
            if (MQTTTelegrafSource::GetPayloadTokenWithValue(cleanPayloadPtr, ",", "bytes_recv", tokenWithValue, sizeof(tokenWithValue)))
            {
                uint64_t bytesRecv = 0;
                if (sscanf(tokenWithValue, "bytes_recv=%" PRIu64, &bytesRecv) == 1)
                {
                    MQTTTelegrafSource::instance->sourceData->SetCurrentNetworkDownload(bytesRecv, currentMessageTimestamp);
                }
                else
                {
#ifdef DEBUG_MQTT_TELEGRAF
                    Serial.println("Error parsing network received bytes value (read eror)");
#endif
                }
            }
            else
            {
#ifdef DEBUG_MQTT_TELEGRAF
                Serial.println("Error parsing network received bytes value (token not found)");
#endif
            }
            if (MQTTTelegrafSource::GetPayloadTokenWithValue(cleanPayloadPtr, ",", "bytes_sent", tokenWithValue, sizeof(tokenWithValue)))
            {
                uint64_t bytesSent = 0;
                if (sscanf(tokenWithValue, "bytes_sent=%" PRIu64, &bytesSent) == 1)
                {
                    MQTTTelegrafSource::instance->sourceData->SetCurrentNetworkUpload(bytesSent, currentMessageTimestamp);
                }
                else
                {
#ifdef DEBUG_MQTT_TELEGRAF
                    Serial.println("Error parsing network sent bytes value (read error)");
#endif
                }
            }
            else
            {
#ifdef DEBUG_MQTT_TELEGRAF
                Serial.println("Error parsing network sent bytes value (token not found)");
#endif
            }
        }
    }
}
