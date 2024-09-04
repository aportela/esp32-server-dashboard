#include "Uint64TFIFO.hpp"
#include <Arduino.h>

namespace aportela::microcontroller::utils
{
    Uint64TFIFO::Uint64TFIFO(size_t size)
        : size(size), count(0), head(0)
    {
        if (this->size == 0)
        {
            Serial.println("Uint64TFIFO: Invalid size (0)");
        }
        else
        {
            this->values = static_cast<uint64_t *>(std::calloc(this->size, sizeof(uint64_t)));
            if (this->values == nullptr)
            {
                Serial.println("Uint64TFIFO: Error allocating memory");
            }
        }
    }

    Uint64TFIFO::~Uint64TFIFO()
    {
        if (this->values != nullptr)
        {
            free(this->values);
            this->values = nullptr;
        }
    }

    void Uint64TFIFO::ShiftLeft(void)
    {
        this->head = (this->head + 1) % this->size;
    }

    size_t Uint64TFIFO::GetSize(void)
    {
        return (this->size);
    }

    size_t Uint64TFIFO::GetCount(void)
    {
        return (this->count);
    }

    size_t Uint64TFIFO::GetHead(void)
    {
        return (this->head);
    }

    uint64_t Uint64TFIFO::GetValueAt(size_t index)
    {
        if (index >= this->count)
        {
            return (0);
        }
        else
        {
            return (values[(this->head + index) % this->size]);
        }
    }

    uint64_t Uint64TFIFO::GetLastValue(void)
    {
        if (this->values != nullptr && this->count > 0)
        {
            return this->values[(this->head + this->count - 1) % this->size];
        }
        else
        {
            return (0);
        }
    }

    uint64_t Uint64TFIFO::GetMaxValue(void)
    {
        if (this->values != nullptr && this->count > 0)
        {
            uint64_t maxValue = this->values[this->head];
            size_t index = this->head;
            for (size_t i = 0; i < this->count; i++)
            {
                if (this->values[index] > maxValue)
                {
                    maxValue = this->values[index];
                }
                index = (index + 1) % this->size;
            }
            return (maxValue);
        }
        else
        {
            return (0);
        }
    }

    bool Uint64TFIFO::Push(uint64_t value)
    {
        if (this->values != nullptr)
        {
            if (this->count < this->size)
            {
                this->values[(this->head + this->count) % this->size] = value;
                this->count++;
            }
            else
            {
                this->ShiftLeft();
                this->values[(this->head + this->size - 1) % this->size] = value;
            }
            return (true);
        }
        else
        {
            return (false);
        }
    }
}
