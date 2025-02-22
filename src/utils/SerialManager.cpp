#include "SerialManager.hpp"
#include <Arduino.h>

namespace aportela::microcontroller::utils
{
    void SerialManager::Init(uint32_t speed)
    {
        Serial.begin(speed);
        while (!Serial && millis() < 500)
        {
            yield();
            delay(10);
        }
    }

    void SerialManager::Loop(const char *commands[], size_t numCommands, SerialCommandCallback callback)
    {
        while (Serial.available() > 0)
        {
            String rx = Serial.readStringUntil('\n');
            if (rx.length() > 0)
            {
                bool validCommand = false;
                for (size_t i = 0; i < numCommands && !validCommand; i++)
                {
                    int commandLength = strlen(commands[i]);
                    if (commandLength > 0)
                    {
                        // comand with params
                        if (commands[i][commandLength - 1] == ' ')
                        {
                            if (rx.startsWith(commands[i]))
                            {
                                if (rx.length() > commandLength)
                                {
                                    validCommand = true;
                                    if (callback != nullptr)
                                    {

                                        callback(i, rx.substring(commandLength).c_str());
                                    }
                                }
                                else
                                {
                                    validCommand = true;
                                    if (callback != nullptr)
                                    {

                                        callback(i, nullptr);
                                    }
                                }
                            }
                        }
                        // single command
                        else
                        {
                            if (rx == commands[i])
                            {
                                validCommand = true;
                                if (callback != nullptr)
                                {
                                    callback(i, nullptr);
                                }
                            }
                        }
                    }
                }
                if (!validCommand)
                {
                    if (callback != nullptr)
                    {
                        callback(-1, rx.c_str());
                    }
                }
            }
        }
    }
}
