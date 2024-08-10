#include "Uint64TFIFO.hpp"
#include <Arduino.h>

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

void Uint64TFIFO::shiftLeft(void)
{
    this->head = (this->head + 1) % this->size;
}

size_t Uint64TFIFO::getSize(void)
{
    return (this->size);
}

size_t Uint64TFIFO::getCount(void)
{
    return (this->count);
}

size_t Uint64TFIFO::getHead(void)
{
    return (this->head);
}

uint64_t Uint64TFIFO::getValueAt(size_t index)
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

uint64_t Uint64TFIFO::getLastValue(void)
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

uint64_t Uint64TFIFO::getMaxValue(void)
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

bool Uint64TFIFO::push(uint64_t value)
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
            this->shiftLeft();
            this->values[(this->head + this->size - 1) % this->size] = value;
        }
        return (true);
    }
    else
    {
        return (false);
    }
}
