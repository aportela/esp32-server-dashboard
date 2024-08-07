#ifndef ESP32_SERVER_DASHBOARD_LGFX_NET_USED_BANDWIDTH_FIFO_VALUES_H
#define ESP32_SERVER_DASHBOARD_LGFX_NET_USED_BANDWIDTH_FIFO_VALUES_H

#include <cstdint>
#include <cstddef>

class Uint64TFIFO
{
private:
    size_t size = 0;
    size_t count = 0;
    size_t head = 0;
    uint64_t *values = nullptr;

    void shiftLeft(void);

public:
    Uint64TFIFO(size_t size);
    ~Uint64TFIFO();
    void push(uint64_t value);
};

#endif
