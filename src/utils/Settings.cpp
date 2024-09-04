#include "Settings.hpp"
#include <cstring>

namespace aportela::microcontroller::utils
{
    Settings::Settings(const char *nameSpace)
    {
        if (nameSpace != NULL && strlen(nameSpace) > 0 && strlen(nameSpace) < SETTINGS_NAMESPACE_ARR_LENGTH)
        {
            strncpy(this->nameSpace, nameSpace, sizeof(this->nameSpace));
            this->preferences = new Preferences();
        }
    }

    Settings::~Settings(void)
    {
        if (this->preferences != nullptr)
        {
            delete this->preferences;
            this->preferences = nullptr;
        }
    }

    bool Settings::GetBoolValue(const char *key, bool defaultValue)
    {
        if (this->preferences != nullptr && this->preferences->begin(this->nameSpace, false))
        {
            bool value = defaultValue;
            if (this->preferences->isKey(key))
            {
                value = this->preferences->getBool(key, defaultValue);
            }
            this->preferences->end();
            return (value);
        }
        else
        {
            return (defaultValue);
        }
    }

    bool Settings::SetBoolValue(const char *key, bool value)
    {
        if (this->preferences != nullptr && this->preferences->begin(this->nameSpace, false))
        {
            bool saved = this->preferences->putBool(key, value) == sizeof(value);
            this->preferences->end();
            return (saved);
        }
        else
        {
            return (false);
        }
    }

    int8_t Settings::GetSmallSignedIntegerValue(const char *key, int8_t defaultValue)
    {
        if (this->preferences != nullptr && this->preferences->begin(this->nameSpace, false))
        {
            uint64_t value = defaultValue;
            if (this->preferences->isKey(key))
            {
                value = this->preferences->getChar(key, defaultValue);
            }
            this->preferences->end();
            return (value);
        }
        else
        {
            return (defaultValue);
        }
    }

    bool Settings::SetSmallSignedIntegerValue(const char *key, int8_t value)
    {
        if (this->preferences != nullptr && this->preferences->begin(this->nameSpace, false))
        {
            bool saved = this->preferences->putChar(key, value) == sizeof(value);
            this->preferences->end();
            return (saved);
        }
        else
        {
            return (false);
        }
    }

    int8_t Settings::GetSmallUnsignedIntegerValue(const char *key, uint8_t defaultValue)
    {
        if (this->preferences != nullptr && this->preferences->begin(this->nameSpace, false))
        {
            uint64_t value = defaultValue;
            if (this->preferences->isKey(key))
            {
                value = this->preferences->getUChar(key, defaultValue);
            }
            this->preferences->end();
            return (value);
        }
        else
        {
            return (defaultValue);
        }
    }

    bool Settings::SetSmallUnsignedIntegerValue(const char *key, uint8_t value)
    {
        if (this->preferences != nullptr && this->preferences->begin(this->nameSpace, false))
        {
            bool saved = this->preferences->putUChar(key, value) == sizeof(value);
            this->preferences->end();
            return (saved);
        }
        else
        {
            return (false);
        }
    }

    uint64_t Settings::GetBigUnsignedIntegerValue(const char *key, uint64_t defaultValue)
    {
        if (this->preferences != nullptr && this->preferences->begin(this->nameSpace, false))
        {
            uint64_t value = defaultValue;
            if (this->preferences->isKey(key))
            {
                value = this->preferences->getULong64(key, defaultValue);
            }
            this->preferences->end();
            return (value);
        }
        else
        {
            return (defaultValue);
        }
    }

    bool Settings::SetBigUnsigedIntegerValue(const char *key, uint64_t value)
    {
        if (this->preferences != nullptr && this->preferences->begin(this->nameSpace, false))
        {
            bool saved = this->preferences->putULong64(key, value) == sizeof(value);
            this->preferences->end();
            return (saved);
        }
        else
        {
            return (false);
        }
    }

    float Settings::GetFloatValue(const char *key, float defaultValue)
    {
        if (this->preferences != nullptr && this->preferences->begin(this->nameSpace, false))
        {
            float value = defaultValue;
            if (this->preferences->isKey(key))
            {
                value = this->preferences->getFloat(key, defaultValue);
            }
            this->preferences->end();
            return (value);
        }
        else
        {
            return (defaultValue);
        }
    }

    bool Settings::SetFloatValue(const char *key, float value)
    {
        if (this->preferences != nullptr && this->preferences->begin(this->nameSpace, false))
        {
            bool saved = this->preferences->putFloat(key, value) == sizeof(value);
            this->preferences->end();
            return (saved);
        }
        else
        {
            return (false);
        }
    }

    void Settings::GetStringValue(const char *key, char *value, size_t count)
    {
        if (this->preferences != nullptr && this->preferences->begin(this->nameSpace, false))
        {
            if (this->preferences->isKey(key))
            {
                this->preferences->getString(key, value, count);
            }
            else
            {
                if (count > 0)
                {
                    value[0] = '\0';
                }
            }
            this->preferences->end();
        }
    }

    bool Settings::SetStringValue(const char *key, const char *value)
    {
        if (this->preferences != nullptr && this->preferences->begin(this->nameSpace, false))
        {
            bool saved = this->preferences->putString(key, value) == strlen(value);
            this->preferences->end();
            return (saved);
        }
        else
        {
            return (false);
        }
    }

    bool Settings::DeleteKey(const char *key)
    {
        if (this->preferences != nullptr && this->preferences->begin(this->nameSpace, false))
        {
            bool removed = false;
            if (this->preferences->isKey(key))
            {
                this->preferences->remove(key);
                removed = true;
            }
            this->preferences->end();
            return (removed);
        }
        else
        {
            return (false);
        }
    }

    bool Settings::Clear(void)
    {
        if (this->preferences != nullptr && this->preferences->begin(this->nameSpace, false))
        {
            bool cleared = this->preferences->clear();
            this->preferences->end();
            return (cleared);
        }
        else
        {
            return (false);
        }
    }
}
