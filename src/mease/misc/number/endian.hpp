#ifndef MEASE_ENDIAN_HPP
#define MEASE_ENDIAN_HPP

#include <concepts>

namespace MEASE
{

template<std::integral T>
constexpr T swapEndian(T src)
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
