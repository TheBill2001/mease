#ifndef MEASE_DATASTREAM_P_HPP
#define MEASE_DATASTREAM_P_HPP

#include "mease/serialize/datastream.hpp"

#include <algorithm>

#include <QDataStream>

using namespace Qt::Literals::StringLiterals;

#define STREAM_READ_MSG(STREAM, TYPE, VAR, MSG)                                                                                                                \
    const auto VAR##Ret = STREAM.read<TYPE>();                                                                                                                 \
    if (!VAR##Ret.has_value()) {                                                                                                                               \
        return errorMsg(MSG, VAR##Ret.error());                                                                                                                \
    }                                                                                                                                                          \
    const auto &VAR = VAR##Ret.value();

#define STREAM_READ_LEN_MSG(STREAM, TYPE, VAR, LEN, MSG)                                                                                                       \
    const auto VAR##Ret = STREAM.read<TYPE>(LEN);                                                                                                              \
    if (!VAR##Ret.has_value()) {                                                                                                                               \
        return errorMsg(MSG, VAR##Ret.error());                                                                                                                \
    }                                                                                                                                                          \
    const auto &VAR = VAR##Ret.value();

#define STREAM_READ_ASSIGN_MSG(STREAM, TYPE, VAR, MSG)                                                                                                         \
    {                                                                                                                                                          \
        const auto ret = STREAM.read<TYPE>();                                                                                                                  \
        if (!ret.has_value()) {                                                                                                                                \
            return errorMsg(MSG, ret.error());                                                                                                                 \
        }                                                                                                                                                      \
        VAR = ret.value();                                                                                                                                     \
    }

#define STREAM_READ_ASSIGN(STREAM, TYPE, VAR)                                                                                                                  \
    {                                                                                                                                                          \
        const auto ret = STREAM.read<TYPE>();                                                                                                                  \
        if (!ret.has_value()) {                                                                                                                                \
            return errorMsg(ret.error());                                                                                                                      \
        }                                                                                                                                                      \
        VAR = ret.value();                                                                                                                                     \
    }

#define STRM_READ_MSG(TYPE, VAR, MSG)                                                                                                                          \
    const auto VAR##Ret = read<TYPE>();                                                                                                                        \
    if (!VAR##Ret.has_value()) {                                                                                                                               \
        return errorMsg(MSG, VAR##Ret.error());                                                                                                                \
    }                                                                                                                                                          \
    const auto &VAR = VAR##Ret.value();

#define STRM_READ_ASSIGN_MSG(TYPE, VAR, MSG)                                                                                                                   \
    {                                                                                                                                                          \
        const auto ret = read<TYPE>();                                                                                                                         \
        if (!ret.has_value()) {                                                                                                                                \
            return errorMsg(MSG, ret.error());                                                                                                                 \
        }                                                                                                                                                      \
        VAR = ret.value();                                                                                                                                     \
    }

#define STRM_READ_LEN_MSG(TYPE, VAR, LEN, MSG)                                                                                                                 \
    const auto VAR##Ret = read<TYPE>(LEN);                                                                                                                     \
    if (!VAR##Ret.has_value()) {                                                                                                                               \
        return errorMsg(MSG, VAR##Ret.error());                                                                                                                \
    }                                                                                                                                                          \
    const auto &VAR = VAR##Ret.value();

#define STRM_READ_LEN_ASSIGN_MSG(TYPE, VAR, LEN, MSG)                                                                                                          \
    {                                                                                                                                                          \
        const auto ret = read<TYPE>(LEN);                                                                                                                      \
        if (!ret.has_value()) {                                                                                                                                \
            return errorMsg(MSG, ret.error());                                                                                                                 \
        }                                                                                                                                                      \
        VAR = ret.value();                                                                                                                                     \
    }

#define STRM_READ_ASSIGN(TYPE, VAR)                                                                                                                            \
    {                                                                                                                                                          \
        const auto ret = read<TYPE>();                                                                                                                         \
        if (!ret.has_value()) {                                                                                                                                \
            return errorMsg(ret.error());                                                                                                                      \
        }                                                                                                                                                      \
        VAR = ret.value();                                                                                                                                     \
    }

#define STRM_READ_LEN_ASSIGN(TYPE, VAR, LEN)                                                                                                                   \
    {                                                                                                                                                          \
        const auto ret = read<TYPE>(LEN);                                                                                                                      \
        if (!ret.has_value()) {                                                                                                                                \
            return errorMsg(ret.error());                                                                                                                      \
        }                                                                                                                                                      \
        VAR = ret.value();                                                                                                                                     \
    }

namespace MEASE
{

class DataStreamPrivate final
{
    Q_DISABLE_COPY(DataStreamPrivate)
    Q_DECLARE_PUBLIC(DataStream)
    DataStream *q_ptr;

public:
    QDataStream stream;

    explicit DataStreamPrivate(QIODevice *device, DataStream *q_ptr);
    explicit DataStreamPrivate(const QByteArray &bytes, DataStream *q_ptr);
    explicit DataStreamPrivate(QByteArray *array, QIODeviceBase::OpenMode mode, DataStream *q_ptr);
};

std::unexpected<QString> errorMsg(QStringView msg, QStringView error);
std::unexpected<QString> errorMsg(QStringView msg);

std::unexpected<QString> unexpected(QStringView error);

template<std::integral T1, std::integral T2>
std::unexpected<QString> notEnoughData(T1 expecting, T2 received)
{
    return std::unexpected(
        QObject::tr("%1, expecting %2, but received %3.")
            .arg(QObject::tr("Not enough data"),
                 QObject::tr("%n byte(s)", "", std::clamp(expecting, T1(std::numeric_limits<int>::min()), T1(std::numeric_limits<int>::max()))),
                 QObject::tr("%n byte(s)", "", std::clamp(received, T2(std::numeric_limits<int>::min()), T2(std::numeric_limits<int>::max())))));
}

template<std::integral T1, std::integral T2>
std::unexpected<QString> notEnoughData(QStringView msg, T1 expecting, T2 received)
{
    return errorMsg(msg,
                    QObject::tr("%1, expecting %2, but received %3.")
                        .arg(QObject::tr("Not enough data"),
                             QObject::tr("%n byte(s)", "", std::clamp(expecting, T1(std::numeric_limits<int>::min()), T1(std::numeric_limits<int>::max()))),
                             QObject::tr("%n byte(s)", "", std::clamp(received, T2(std::numeric_limits<int>::min()), T2(std::numeric_limits<int>::max())))));
}

template<std::integral T1, std::integral T2>
std::unexpected<QString> unsupportedVersion(QStringView msg, T1 expecting, T2 received)
{
    return errorMsg(
        msg,
        QObject::tr("%1, expecting %2, but received %3.").arg(QObject::tr("Unsupported version"), QString::number(expecting), QString::number(received)));
}

template<std::integral T1, std::integral T2>
std::unexpected<QString> checksumMismatch(T1 expecting, T2 received)
{
    return std::unexpected(
        QObject::tr("%1, expecting %2, but received %3.").arg(QObject::tr("Checksum mismatch"), QString::number(expecting, 16), QString::number(received, 16)));
}

template<std::integral T1, std::integral T2>
std::unexpected<QString> checksumMismatch(QStringView msg, T1 expecting, T2 received)
{
    return errorMsg(
        msg,
        QObject::tr("%1, expecting %2, but received %3.").arg(QObject::tr("Checksum mismatch"), QString::number(expecting, 16), QString::number(received, 16)));
}

template<std::integral T1, std::integral T2>
std::unexpected<QString> invalidSignature(T1 expecting, T2 received)
{
    return std::unexpected(QObject::tr("%1, expecting %2, but received %3.")
                               .arg(QObject::tr("Invalid signature"),
                                    u"0x%1"_s.arg(QString::number(expecting, 16).toUpper()),
                                    u"0x%1"_s.arg(QString::number(received, 16)).toUpper()));
}

template<std::integral T1, std::integral T2>
std::unexpected<QString> invalidSignature(QStringView msg, T1 expecting, T2 received)
{
    return errorMsg(msg,
                    QObject::tr("%1, expecting %2, but received %3.")
                        .arg(QObject::tr("Invalid signature"),
                             u"0x%1"_s.arg(QString::number(expecting, 16).toUpper()),
                             u"0x%1"_s.arg(QString::number(received, 16)).toUpper()));
}

} // namespace MEASE

#endif // MEASE_DATASTREAM_P_HPP
