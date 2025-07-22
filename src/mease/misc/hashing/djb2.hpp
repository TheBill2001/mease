#ifndef MEASE_MODEFILEDDJB2_HPP
#define MEASE_MODEFILEDDJB2_HPP

#include <QtTypes>

namespace MEASE
{

constexpr quint32 djbHash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash *= 33;
        hash += c;
    }
    return hash;
}

constexpr quint32 modifiedDjbHash(const char *str)
{
    uint32_t hash = 5381;
    uint8_t c;

    while ((c = *str++)) {
        hash *= 33;
        hash ^= c;
    }

    return hash;
}

} // namespace MEASE

#endif // MEASE_MODEFILEDDJB2_HPP
