#include "MQTTTelegrafSource.hpp"
#include "../../utils/MQTT.hpp"
#include <cstring>
#include <inttypes.h>
#include <Arduino.h>

// char MQTTTelegrafSource::cpuTopic[MAX_TOPIC_LENGTH] = {'\0'};
// char MQTTTelegrafSource::memTopic[MAX_TOPIC_LENGTH] = {'\0'};

MQTTTelegrafSource *MQTTTelegrafSource::instance = nullptr;
MQTTTelegrafSource::MQTTTelegrafSource(const char *uri, const char *clientId, const char *topic) : Source()
{

    MQTT::setCallback(MQTTTelegrafSource::onMessageReceived);
    MQTT::init(clientId, uri, topic);

    this->currentCPULoad = new EntityData(MIN_CPU_LOAD, 100);
    this->currentCPULoad->setCurrent(MIN_CPU_LOAD, millis());

    this->currentMemory = new EntityData(MIN_MEMORY, MAX_MEMORY);
    this->currentMemory->setCurrent(MIN_CPU_LOAD, millis());

    this->currentCPUTemperature = new EntityData(MIN_CPU_TEMPERATURE, MAX_CPU_TEMPERATURE);
    this->currentCPUTemperature->setCurrent(MIN_CPU_TEMPERATURE, millis());

    this->currentNetworkDownloadBandwith = new EntityData(MIN_NETWORK_DOWNLOAD_BANDWITH, MAX_NETWORK_DOWNLOAD_BANDWITH);
    this->currentNetworkDownloadBandwith->setCurrent(MIN_NETWORK_DOWNLOAD_BANDWITH, millis());

    this->currentNetworkUploadBandwith = new EntityData(MIN_NETWORK_UPLOAD_BANDWITH, MAX_NETWORK_UPLOAD_BANDWITH);
    this->currentNetworkUploadBandwith->setCurrent(MIN_NETWORK_UPLOAD_BANDWITH, millis());

    MQTTTelegrafSource::instance = this;
}

MQTTTelegrafSource::~MQTTTelegrafSource()
{
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
    // TODO: read real timestamps
    if (strcmp(topic, "telegraf/HOST_NAME/cpu") == 0 && strncmp(payload, "cpu,cpu=cpu-total", 17) == 0)
    {

        float usage_idle = 0.0;
        const char *search = "usage_idle=";
        const char *start = strstr(payload, search);
        if (start)
        {
            char subPayload[strlen(payload)];
            strncpy(subPayload, start, sizeof(subPayload) - 1);
            subPayload[sizeof(subPayload) - 1] = '\0'; // Asegurar la terminación nula
            const char *format = "usage_idle=%f";
            if (sscanf(subPayload, format, &usage_idle) == 1)
            {
                float cpu_usage = 100.0 - usage_idle; // Porcentaje de uso total de la CPU
                // Serial.printf("Total CPU Usage: %.2f%%\n", cpu_usage);
                // Serial.printf("Total CPU Usage: %d%%\n", (uint8_t)cpu_usage);
                MQTTTelegrafSource::instance->currentCPULoad->setCurrent((uint8_t)cpu_usage, millis());
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
            subPayload[sizeof(subPayload) - 1] = '\0'; // Asegurar la terminación nula
            const char *format = "used=%" PRIu64 "i";

            if (sscanf(subPayload, format, &used) == 1)
            {
                MQTTTelegrafSource::instance->currentMemory->setCurrent(used, millis());
            }
            else
            {
                Serial.println("Failed to parse payload");
            }
            // TODO SET TOTAL ?
        }
    }
    else if (false && strcmp(topic, "telegraf/HOST_NAME/net") == 0 && strncmp(payload, "net,host=HOST_NAME,interface=Ethernet\ 10G\ (SFP+)", 20) == 0)
    {
        if (strstr(payload, "interface=Wi-Fi"))
        {
            Serial.println(payload);
            uint64_t recv = 0;
            const char *searchRX = "bytes_recv=";
            const char *startRX = strstr(payload, searchRX);
            if (startRX)
            {
                char subPayload[strlen(payload)];
                strncpy(subPayload, startRX, sizeof(subPayload) - 1);
                subPayload[sizeof(subPayload) - 1] = '\0'; // Asegurar la terminación nula
                const char *format = "bytes_recv=%" PRIu64 "i";

                if (sscanf(subPayload, format, &recv) == 1)
                {
                    Serial.print("RECV: ");
                    Serial.println(recv);
                    uint64_t lastTimestamp = MQTTTelegrafSource::instance->currentNetworkDownloadBandwith->getCurrentTimestamp();
                    uint64_t currentTimestamp = millis();
                    uint64_t diff = recv - MQTTTelegrafSource::instance->currentNetworkDownloadBandwith->getCurrent();
                    Serial.print("DIFF: ");
                    Serial.println(diff);
                    Serial.print("DIFF / ellapsed: ");
                    uint64_t elapsedMillis = (currentTimestamp - lastTimestamp);
                    Serial.println(diff / elapsedMillis);
                    Serial.print("Ellapsed millis: ");
                    Serial.println(elapsedMillis);
                    MQTTTelegrafSource::instance->currentNetworkDownloadBandwith->setCurrent(diff / elapsedMillis, currentTimestamp);
                }
                else
                {
                    Serial.println("Failed to parse RX payload");
                }
                // TODO SET TOTAL ?
            }
            uint64_t sent = 0;
            const char *searchTX = "bytes_sent=";
            const char *startTX = strstr(payload, searchTX);
            if (startTX)
            {
                char subPayload[strlen(payload)];
                strncpy(subPayload, startTX, sizeof(subPayload) - 1);
                subPayload[sizeof(subPayload) - 1] = '\0'; // Asegurar la terminación nula
                const char *format = "bytes_sent=%" PRIu64 "i";

                if (sscanf(subPayload, format, &sent) == 1)
                {
                    Serial.print("SENT: ");
                    Serial.println(sent);
                    uint64_t lastTimestamp = MQTTTelegrafSource::instance->currentNetworkUploadBandwith->getCurrentTimestamp();
                    uint64_t currentTimestamp = millis();
                    uint64_t diff = sent - MQTTTelegrafSource::instance->currentNetworkUploadBandwith->getCurrent();
                    Serial.print("DIFF: ");
                    Serial.println(diff);
                    Serial.print("DIFF / ellapsed: ");
                    uint64_t elapsedMillis = (currentTimestamp - lastTimestamp);
                    Serial.println(diff / elapsedMillis);
                    Serial.print("Ellapsed millis: ");
                    Serial.println(elapsedMillis);
                    MQTTTelegrafSource::instance->currentNetworkUploadBandwith->setCurrent(diff / elapsedMillis, currentTimestamp);
                }
                else
                {
                    Serial.println("Failed to parse TX payload");
                }
                // TODO SET TOTAL ?
            }
        }
    }
}

uint64_t MQTTTelegrafSource::getCurrentCPULoad(void)
{
    return (this->currentCPULoad->getCurrent());
}

uint64_t MQTTTelegrafSource::getTotalMemory(void)
{
    return (this->currentMemory->getMax());
}

uint64_t MQTTTelegrafSource::getUsedMemory(void)
{
    return (this->currentMemory->getCurrent());
}

uint64_t MQTTTelegrafSource::getCurrentCPUTemperature(void)
{
    return (this->currentCPUTemperature->getCurrent());
}

uint64_t MQTTTelegrafSource::getTotalNetworkDownloadBandwith(void)
{
    return (this->currentNetworkDownloadBandwith->getMax());
}

uint64_t MQTTTelegrafSource::getUsedNetworkDownloadBandwith(void)
{
    return (this->currentNetworkDownloadBandwith->getCurrent());
}

uint64_t MQTTTelegrafSource::getTotalNetworkUploadBandwith(void)
{
    return (this->currentNetworkUploadBandwith->getMax());
}

uint64_t MQTTTelegrafSource::getUsedNetworkUploadBandwith(void)
{
    return (this->currentNetworkUploadBandwith->getCurrent());
}
