#ifndef MEASE_ENDIAN_HPP
#define MEASE_ENDIAN_HPP

#include <array>
#include <concepts>
#include <cstring>

namespace MEASE
{

template<typename T>
consteval T swapEndian(T src) = delete;

template<std::integral T>
consteval T swapEndian(T src)
{
    T result = 0;
    for (std::size_t i = 0; i < sizeof(T); ++i) {
        result = (result << 8) | (src & 0xFFu);
        src >>= 8;
    }
    return result;
}

} // namespace MEASE

#endif // MEASE_ENDIAN_HPP
