#ifndef MEASE_SERIALIZER_P_HPP
#define MEASE_SERIALIZER_P_HPP

#include "mease/serialize/serializer.hpp"

#include <QIODevice>

#include <KLazyLocalizedString>

namespace MEASE
{
inline constexpr auto crc2Seed = 0x12345678;

class SerializerPrivate
{
    Q_DECLARE_PUBLIC(Serializer)
    Serializer *q_ptr;

public:
    bool ownDevice = false;
    QIODevice *device = nullptr;
    Serializer::ByteOrder byteOrder = Serializer::LittleEndian;

    explicit SerializerPrivate(QIODevice *d, Serializer *q_ptr)
        : q_ptr{q_ptr}
        , device{d}
    {
    }

    ~SerializerPrivate()
    {
        if (ownDevice && device) {
            delete device;
        }
    }
};

inline constexpr auto invalidFloatNumber = kli18n("Invalid floating point number");
inline constexpr auto invalidIntegralNumber = kli18n("Invalid integral number");
} // namespace MEASE

#define READ(TYPE, NAME)                                                                                                                                       \
    TYPE NAME{};                                                                                                                                               \
    {                                                                                                                                                          \
        auto res = read<TYPE>();                                                                                                                               \
        if (!res.has_value()) {                                                                                                                                \
            return std::unexpected(std::move(res.error()));                                                                                                    \
        }                                                                                                                                                      \
        NAME = std::move(res.value());                                                                                                                         \
    }

#define READ_LENGTH(TYPE, NAME, LENGTH)                                                                                                                        \
    TYPE NAME{};                                                                                                                                               \
    {                                                                                                                                                          \
        auto res = read<TYPE>(LENGTH);                                                                                                                         \
        if (!res.has_value()) {                                                                                                                                \
            return std::unexpected(std::move(res.error()));                                                                                                    \
        }                                                                                                                                                      \
        NAME = std::move(res.value());                                                                                                                         \
    }

#define READ_INTO(TYPE, INTO)                                                                                                                                  \
    {                                                                                                                                                          \
        auto res = read<TYPE>();                                                                                                                               \
        if (!res.has_value()) {                                                                                                                                \
            return std::unexpected(std::move(res.error()));                                                                                                    \
        }                                                                                                                                                      \
        INTO = std::move(res.value());                                                                                                                         \
    }

#define READ_LENGTH_INTO(TYPE, INTO, LENGTH)                                                                                                                   \
    {                                                                                                                                                          \
        auto res = read<TYPE>(LENGTH);                                                                                                                         \
        if (!res.has_value()) {                                                                                                                                \
            return std::unexpected(std::move(res.error()));                                                                                                    \
        }                                                                                                                                                      \
        INTO = std::move(res.value());                                                                                                                         \
    }

#define SRLZR_READ(TYPE, NAME, SERIALIZER)                                                                                                                     \
    TYPE NAME{};                                                                                                                                               \
    {                                                                                                                                                          \
        auto res = SERIALIZER.read<TYPE>();                                                                                                                    \
        if (!res.has_value()) {                                                                                                                                \
            return std::unexpected(std::move(res.error()));                                                                                                    \
        }                                                                                                                                                      \
        NAME = std::move(res.value());                                                                                                                         \
    }

#define SRLZR_READ_LENGTH(TYPE, NAME, SERIALIZER, LENGTH)                                                                                                      \
    TYPE NAME{};                                                                                                                                               \
    {                                                                                                                                                          \
        auto res = SERIALIZER.read<TYPE>(LENGTH);                                                                                                              \
        if (!res.has_value()) {                                                                                                                                \
            return std::unexpected(std::move(res.error()));                                                                                                    \
        }                                                                                                                                                      \
        NAME = std::move(res.value());                                                                                                                         \
    }

#define SRLZR_READ_INTO(TYPE, INTO, SERIALIZER)                                                                                                                \
    {                                                                                                                                                          \
        auto res = SERIALIZER.read<TYPE>();                                                                                                                    \
        if (!res.has_value()) {                                                                                                                                \
            return std::unexpected(std::move(res.error()));                                                                                                    \
        }                                                                                                                                                      \
        INTO = std::move(res.value());                                                                                                                         \
    }

#define SRLZR_READ_LENGTH_INTO(TYPE, INTO, SERIALIZER, LENGTH)                                                                                                 \
    {                                                                                                                                                          \
        auto res = SERIALIZER.read<TYPE>(LENGTH);                                                                                                              \
        if (!res.has_value()) {                                                                                                                                \
            return std::unexpected(std::move(res.error()));                                                                                                    \
        }                                                                                                                                                      \
        INTO = std::move(res.value());                                                                                                                         \
    }

#endif // MEASE_SERIALIZER_P_HPP
