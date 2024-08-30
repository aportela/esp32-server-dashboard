#ifndef APORTELA_MICROCONTROLLER_UTILS_SETTINGS_H
#define APORTELA_MICROCONTROLLER_UTILS_SETTINGS_H

#include <cstddef>
#include <Preferences.h>

// WARNING: namespaces with length > 15 WILL NOT OPEN NVS CORRECTLY
// WARNING: ALSO keys with length > 15 DO NOT WORK

#define SETTINGS_NAMESPACE_ARR_LENGTH 16 // (15 chars + '\0')

namespace aportela::microcontroller::utils
{
    class Settings
    {
    protected:
        Preferences *preferences;
        char nameSpace[SETTINGS_NAMESPACE_ARR_LENGTH];

        bool getBoolValue(const char *key, bool defaultValue = false);
        bool setBoolValue(const char *key, bool value = false);
        int8_t getSmallSignedIntegerValue(const char *key, int8_t defaultValue = 0);
        bool setSmallSignedIntegerValue(const char *key, int8_t value = 0);
        int8_t getSmallUnsignedIntegerValue(const char *key, uint8_t defaultValue = 0);
        bool setSmallUnsignedIntegerValue(const char *key, uint8_t value = 0);
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
}

#endif // APORTELA_MICROCONTROLLER_UTILS_SETTINGS_H
