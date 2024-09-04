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

        bool GetBoolValue(const char *key, bool defaultValue = false);
        bool SetBoolValue(const char *key, bool value = false);
        int8_t GetSmallSignedIntegerValue(const char *key, int8_t defaultValue = 0);
        bool SetSmallSignedIntegerValue(const char *key, int8_t value = 0);
        int8_t GetSmallUnsignedIntegerValue(const char *key, uint8_t defaultValue = 0);
        bool SetSmallUnsignedIntegerValue(const char *key, uint8_t value = 0);
        uint64_t GetBigUnsignedIntegerValue(const char *key, uint64_t defaultValue = 0);
        bool SetBigUnsigedIntegerValue(const char *key, uint64_t value = 0);
        float GetFloatValue(const char *key, float defaultValue = 0.0f);
        bool SetFloatValue(const char *key, float value = 0.0f);
        void GetStringValue(const char *key, char *value, size_t count);
        bool SetStringValue(const char *key, const char *value);
        bool DeleteKey(const char *key);

    public:
        Settings(const char *nameSpace);
        ~Settings();

        bool Clear(void);
    };
}

#endif // APORTELA_MICROCONTROLLER_UTILS_SETTINGS_H
