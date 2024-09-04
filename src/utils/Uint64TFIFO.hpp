#ifndef APORTELA_MICROCONTROLLER_UTILS_UINT64TFIFO_H
#define APORTELA_MICROCONTROLLER_UTILS_UINT64TFIFO_H

#include <cstdint>
#include <cstddef>

namespace aportela::microcontroller::utils
{
    class Uint64TFIFO
    {
    private:
        size_t size = 0;
        size_t count = 0;
        size_t head = 0;
        uint64_t *values = nullptr;
        void ShiftLeft(void);

    public:
        Uint64TFIFO(size_t size);
        ~Uint64TFIFO();
        size_t GetSize(void);
        size_t GetCount(void);
        size_t GetHead(void);
        uint64_t GetValueAt(size_t index);
        uint64_t GetLastValue(void);
        uint64_t GetMaxValue(void);
        bool Push(uint64_t value);
    };
}

#endif // APORTELA_MICROCONTROLLER_UTILS_UINT64TFIFO_H
