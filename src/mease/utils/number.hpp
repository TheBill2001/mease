// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_NUMBER_HPP
#define MEASE_NUMBER_HPP

#include <concepts>

#include <QString>

namespace MEASE
{
template<typename T>
consteval T swapEndian(T src) = delete;

template<std::unsigned_integral T>
consteval T swapEndian(T src)
{
    T result = 0;
    for (std::size_t i = 0; i < sizeof(T); ++i) {
        result = (result << 8) | (src & 0xFFu);
        src >>= 8;
    }
    return result;
}

template<std::integral T>
QString toHex(const T &value, int length = 8)
{
    return QStringLiteral("0x%1").arg(value, length, 16, QLatin1Char('0'));
}

template<std::floating_point T>
QString toHex(const T &value, int length = 8)
{
    return QStringLiteral("0x%1").arg(QString::fromLatin1(QByteArray(reinterpret_cast<const char *>(value), sizeof(T)).toHex(0)), length, QLatin1Char('0'));
}
} // namespace MEASE

#endif // MEASE_NUMBER_HPP
