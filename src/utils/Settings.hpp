#ifndef ESP32_SERVER_DASHBOARD_SETTINGS_H
#define ESP32_SERVER_DASHBOARD_SETTINGS_H

#include <cstddef>
#include <Preferences.h>

// WARNING: namespaces with length > 15 WILL NOT OPEN NVS CORRECTLY
// WARNING: ALSO keys with length > 15 DO NOT WORK

#define MAX_SETTINGS_NAMESPACE_LENGTH 16 // (15 chars + '\0')

class Settings
{
protected:
    Preferences *preferences;
    char nameSpace[MAX_SETTINGS_NAMESPACE_LENGTH];

    int8_t getSmallSignedIntegerValue(const char *key, int8_t defaultValue = 0);
    bool setSmallSignedIntegerValue(const char *key, int8_t value = 0);
    uint64_t getBigUnsignedIntegerValue(const char *key, uint64_t defaultValue = 0);
    bool setBigUnsigedIntegerValue(const char *key, uint64_t value = 0);
    float getFloatValue(const char *key, float defaultValue = 0.0f);
    bool setFloatValue(const char *key, float value = 0.0f);
    void getStringValue(const char *key, char *value, size_t count);
    bool setStringValue(const char *key, const char *value);
    bool deleteKey(const char *key);

public:
    Settings(const char *nameSpace);
    ~Settings();

    bool clear(void);
};

#endif
