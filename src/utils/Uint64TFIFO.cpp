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
    for (size_t i = 0; i < this->size - 1; i++)
    {
        this->values[i] = this->values[(this->head + i + 1) % this->size];
    }
    this->head = (this->head + 1) % this->size;
}

void Uint64TFIFO::push(uint64_t value)
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
}
