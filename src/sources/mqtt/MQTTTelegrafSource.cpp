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

uint64_t MQTTTelegrafSource::GetPayloadTimestamp(const char *payload)
{
    uint64_t timestamp = 0;
    const char *lastSpaceSubStr = strrchr(payload, ' ');
    if (lastSpaceSubStr == NULL || lastSpaceSubStr == payload + strlen(payload) - 1)
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
            timestamp = payloadTimestamp / 1000000;
        }
        else
        {
#ifdef DEBUG_MQTT_TELEGRAF
            Serial.println("Error parsing payload timestamp");
#endif
        }
    }
    return (timestamp);
}

void MQTTTelegrafSource::GetPayloadTokens(const char *payload, char *buffer, size_t bufferSize)
{
    const char *lastSpace = nullptr;        // last real whitespace position pointer
    const char *penultimateSpace = nullptr; // penultimate real whitespace position pointer
    bool escape = false;

    // foreach input string chars
    for (const char *p = payload; *p != '\0'; ++p)
    {
        if (*p == '\\' && !escape)
        {
            // invert slash found => enable escape flag
            escape = true;
        }
        else
        {
            if (*p == ' ' && !escape)
            {
                // real whitespace found (not escaped)
                penultimateSpace = lastSpace; // update penultimate real whitespace position pointer
                lastSpace = p;                // update last real whitespace position pointer
            }
            else
            {
                escape = false; // reset escape flag with another chars
            }
        }
    }

    const char *penultimateTokenStart = (penultimateSpace != nullptr) ? penultimateSpace + 1 : payload;
    const char *penultimateTokenEnd = (lastSpace != nullptr) ? lastSpace : payload + strlen(payload);

    if (bufferSize > 0)
    {
        size_t tokenLength = penultimateTokenEnd - penultimateTokenStart;
        size_t copyLength = (tokenLength < bufferSize - 1) ? tokenLength : bufferSize - 1;
        strncpy(buffer, penultimateTokenStart, copyLength);
        buffer[copyLength] = '\0';
    }
}

void MQTTTelegrafSource::OnMessageReceived(const char *topic, const char *payload)
{
#ifdef DEBUG_MQTT_TELEGRAF
    Serial.print("Topic:");
    Serial.println(topic);
    Serial.print("Message:");
    Serial.println(payload);
#endif
    uint64_t currentMessageTimestamp = MQTTTelegrafSource::GetPayloadTimestamp(payload);
    // by default use current esp32 timestamp (failover for payload timestamp parse errors)
    if (currentMessageTimestamp == 0)
    {
        currentMessageTimestamp = millis();
    }
    char cleanPayload[strlen(payload)] = {'\0'};
    // this method is used because payload can have escaped whitespaces (ex: interface=Ethernet\ 10G\ (SFP+))
    // and token split with strok do not work as spected so we need to parse raw payload & get "values relevant" substring
    // NOTE: this will IGNORE all tokens before escaped whitespaces but for this code works fine because on the received
    // payload key=value required tokens always are at the end, before timestamp
    // example: net,host=WINDOWS,interface=Ethernet\ 10G\ (SFP+) bytes_sent=40845899802i,bytes_recv=357515820769i,packets_recv=308742870i,drop_in=62i,speed=-1i,packets_sent=115628425i,err_in=62i,err_out=0i,drop_out=0i 1725955392000000000
    // (I only want from bytes_sent=... to end)
    MQTTTelegrafSource::GetPayloadTokens(payload, cleanPayload, sizeof(cleanPayload));
    char tokenWithValue[strlen(payload)] = {'\0'};
    if (strcmp(topic, MQTTTelegrafSource::cpuTopic) == 0 && strncmp(payload, "cpu,cpu=cpu-total", 17) == 0)
    {
        SourceDataQueueCPUValues currentData;
        currentData.timestamp = currentMessageTimestamp;
        uint8_t totalValidTokens = 0;
        char *tmpPayload = strdup(cleanPayload);
        if (tmpPayload != NULL)
        {
            char *token = strtok(tmpPayload, ",");
            while (token != NULL)
            {
                if (strncmp(token, "usage_user=", strlen("usage_user=")) == 0)
                {
                    if (sscanf(token, "usage_user=%f", &currentData.usageUser) == 1)
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
                else if (strncmp(token, "usage_system=", strlen("usage_system=")) == 0)
                {
                    if (sscanf(token, "usage_system=%f", &currentData.usageSystem) == 1)
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
                else if (strncmp(token, "usage_idle=", strlen("usage_idle=")) == 0)
                {
                    if (sscanf(token, "usage_idle=%f", &currentData.usageIdle) == 1)
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
                else if (strncmp(token, "usage_active=", strlen("usage_active=")) == 0)
                {
                    if (sscanf(token, "usage_active=%f", &currentData.usageActive) == 1)
                    {
                        totalValidTokens++;
                    }
                    else
                    {
#ifdef DEBUG_MQTT_TELEGRAF
                        Serial.println("Error parsing CPU usage active value (read error)");
#endif
                    }
                }
                else if (strncmp(token, "usage_nice=", strlen("usage_nice=")) == 0)
                {
                    if (sscanf(token, "usage_nice=%f", &currentData.usageNice) == 1)
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
                    if (sscanf(token, "usage_iowait=%f", &currentData.usageIOWait) == 1)
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
                    if (sscanf(token, "usage_irq=%f", &currentData.usageIRQ) == 1)
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
                    if (sscanf(token, "usage_softirq=%f", &currentData.usageSoftIRQ) == 1)
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
                else if (strncmp(token, "usage_steal=", strlen("usage_steal=")) == 0)
                {
                    if (sscanf(token, "usage_steal=%f", &currentData.usageSteal) == 1)
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
                else if (strncmp(token, "usage_guest=", strlen("usage_guest=")) == 0)
                {
                    if (sscanf(token, "usage_guest=%f", &currentData.usageGuest) == 1)
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
                    if (sscanf(token, "usage_guest_nice=%f", &currentData.usageGuestNice) == 1)
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
                token = strtok(NULL, ",");
            }
            free(tmpPayload);
        }
        if (totalValidTokens > 0)
        {
            MQTTTelegrafSource::instance->sourceData->SetCurrentCPUData(currentData);
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
        char *tmpPayload = strdup(cleanPayload);
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
            if (MQTTTelegrafSource::GetPayloadTokenWithValue(cleanPayload, ",", "temp_input", tokenWithValue, sizeof(tokenWithValue)))
            {
                SourceDataQueueCPUTemperatureValue currentData;
                currentData.timestamp = currentMessageTimestamp;
                if (sscanf(tokenWithValue, "temp_input=%f", &currentData.celsious) == 1)
                {
                    MQTTTelegrafSource::instance->sourceData->SetCurrentCPUTemperature(currentData);
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
        if (MQTTTelegrafSource::GetPayloadTokenWithValue(cleanPayload, ",", "temp", tokenWithValue, sizeof(tokenWithValue)))
        {
            SourceDataQueueCPUTemperatureValue currentData;
            currentData.timestamp = currentMessageTimestamp;
            if (sscanf(tokenWithValue, "temp=%f", &currentData.celsious) == 1)
            {
                MQTTTelegrafSource::instance->sourceData->SetCurrentCPUTemperature(currentData);
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
        if (MQTTTelegrafSource::GetPayloadTokenWithValue(cleanPayload, ",", "uptime", tokenWithValue, sizeof(tokenWithValue)))
        {
            SourceDataQueueUptimeValue currentData;
            currentData.timestamp = currentMessageTimestamp;
            // WARNING: (BEFORE REPORTING BUG) WINDOWS REPORTS "INVALID" UPTIME IF YOU ARE USING "FAST STARTUP" SO IF YOUR SERVER UPTIME SEEMS TO BE WRONG CHECK IF YOU HAVE THIS FEATURE ENABLED
            // https://answers.microsoft.com/en-us/windows/forum/all/fast-startup-doesnt-break-power-cycle/07bd6bf8-dd24-4c77-b911-17df89c74eb3
            if (sscanf(tokenWithValue, "uptime=%" PRIu64, &currentData.seconds) == 1)
            {
                MQTTTelegrafSource::instance->sourceData->SetCurrentUptime(currentData);
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
            SourceDataQueueNetworkingValue currentData;
            currentData.timestamp = currentMessageTimestamp;
            uint8_t totalValidTokens = 0;
            char *tmpPayload = strdup(cleanPayload);
            if (tmpPayload != NULL)
            {
                char *token = strtok(tmpPayload, ",");
                while (token != NULL)
                {
                    if (strncmp(token, "bytes_sent=", strlen("bytes_sent=")) == 0)
                    {
                        if (sscanf(token, "bytes_sent=%" PRIu64, &currentData.bytesSent) == 1)
                        {
                            totalValidTokens++;
                        }
                        else
                        {
#ifdef DEBUG_MQTT_TELEGRAF
                            Serial.println("Error parsing network bytes sent value (read error)");
#endif
                        }
                    }
                    else if (strncmp(token, "bytes_recv=", strlen("bytes_recv=")) == 0)
                    {
                        if (sscanf(token, "bytes_recv=%" PRIu64, &currentData.bytesRecv) == 1)
                        {
                            totalValidTokens++;
                        }
                        else
                        {
#ifdef DEBUG_MQTT_TELEGRAF
                            Serial.println("Error parsing network bytes recv value (read error)");
#endif
                        }
                    }
                    else if (strncmp(token, "packets_sent=", strlen("packets_sent=")) == 0)
                    {
                        if (sscanf(token, "packets_sent=%" PRIu64, &currentData.packetsSent) == 1)
                        {
                            totalValidTokens++;
                        }
                        else
                        {
#ifdef DEBUG_MQTT_TELEGRAF
                            Serial.println("Error parsing network packets sent value (read error)");
#endif
                        }
                    }
                    else if (strncmp(token, "packets_recv=", strlen("packets_recv=")) == 0)
                    {
                        if (sscanf(token, "packets_recv=%" PRIu64, &currentData.packetsRecv) == 1)
                        {
                            totalValidTokens++;
                        }
                        else
                        {
#ifdef DEBUG_MQTT_TELEGRAF
                            Serial.println("Error parsing network packets recv value (read error)");
#endif
                        }
                    }
                    else if (strncmp(token, "err_in=", strlen("err_in=")) == 0)
                    {
                        if (sscanf(token, "err_in=%" PRIu64, &currentData.errIn) == 1)
                        {
                            totalValidTokens++;
                        }
                        else
                        {
#ifdef DEBUG_MQTT_TELEGRAF
                            Serial.println("Error parsing network err in value (read error)");
#endif
                        }
                    }
                    else if (strncmp(token, "err_out=", strlen("err_out=")) == 0)
                    {
                        if (sscanf(token, "err_out=%" PRIu64, &currentData.errOut) == 1)
                        {
                            totalValidTokens++;
                        }
                        else
                        {
#ifdef DEBUG_MQTT_TELEGRAF
                            Serial.println("Error parsing network err out value (read error)");
#endif
                        }
                    }
                    else if (strncmp(token, "drop_in=", strlen("drop_in=")) == 0)
                    {
                        if (sscanf(token, "drop_in=%" PRIu64, &currentData.dropIn) == 1)
                        {
                            totalValidTokens++;
                        }
                        else
                        {
#ifdef DEBUG_MQTT_TELEGRAF
                            Serial.println("Error parsing network drop in value (read error)");
#endif
                        }
                    }
                    else if (strncmp(token, "drop_out=", strlen("drop_out=")) == 0)
                    {
                        if (sscanf(token, "drop_out=%" PRIu64, &currentData.dropOut) == 1)
                        {
                            totalValidTokens++;
                        }
                        else
                        {
#ifdef DEBUG_MQTT_TELEGRAF
                            Serial.println("Error parsing network drop out value (read error)");
#endif
                        }
                    }
                    else if (strncmp(token, "speed=", strlen("speed=")) == 0)
                    {
                        if (sscanf(token, "speed=%" PRIu64, &currentData.speed) == 1)
                        {
                            totalValidTokens++;
                        }
                        else
                        {
#ifdef DEBUG_MQTT_TELEGRAF
                            Serial.println("Error parsing network speed value (read error)");
#endif
                        }
                    }

                    token = strtok(NULL, ",");
                }
                free(tmpPayload);
            }
            if (totalValidTokens > 0)
            {
                MQTTTelegrafSource::instance->sourceData->SetCurrentNetwork(currentData);
            }
            else
            {
#ifdef DEBUG_MQTT_TELEGRAF
                Serial.printf("Error parsing network values (tokens found: %d of %d)\n", totalValidTokens, 10);
#endif
            }
        }
    }
}
