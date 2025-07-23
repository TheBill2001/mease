#include "mease/serialize/datastream_p.hpp"

#include <concepts>

#include <QObject>

namespace MEASE
{

std::unexpected<QString> errorMsg(QStringView msg, QStringView error)
{
    return std::unexpected(u"%1: %2"_s.arg(msg, error));
}

std::unexpected<QString> errorMsg(QStringView msg)
{
    return std::unexpected(msg.toString());
}

std::unexpected<QString> unexpected(QStringView error)
{
    return std::unexpected(QObject::tr("Unexpected error: %1").arg(error));
}

DataStreamPrivate::DataStreamPrivate(QIODevice *device, DataStream *q_ptr)
    : q_ptr{q_ptr}
    , stream{device}
{
    stream.setByteOrder(QDataStream::LittleEndian);
}

DataStreamPrivate::DataStreamPrivate(const QByteArray &bytes, DataStream *q_ptr)
    : q_ptr{q_ptr}
    , stream{bytes}
{
    stream.setByteOrder(QDataStream::LittleEndian);
}

DataStreamPrivate::DataStreamPrivate(QByteArray *array, QIODeviceBase::OpenMode mode, DataStream *q_ptr)
    : q_ptr{q_ptr}
    , stream{array, mode}
{
    stream.setByteOrder(QDataStream::LittleEndian);
}

DataStream::DataStream(QIODevice *device)
    : d_ptr{new DataStreamPrivate(device, this)}
{
}

DataStream::DataStream(const QByteArray &bytes)
    : d_ptr{new DataStreamPrivate(bytes, this)}
{
}

DataStream::DataStream(QByteArray *array, QIODeviceBase::OpenMode mode)
    : d_ptr{new DataStreamPrivate(array, mode, this)}
{
}

DataStream::~DataStream() = default;

DataStream::ByteOrder DataStream::byteOrder() const
{
    Q_D(const DataStream);
    return static_cast<DataStream::ByteOrder>(d->stream.byteOrder());
}

void DataStream::setByteOrder(ByteOrder byteOrder)
{
    Q_D(DataStream);
    d->stream.setByteOrder(static_cast<QDataStream::ByteOrder>(byteOrder));
}

template<>
DataStream::Result<QByteArray> DataStream::read<QByteArray>(qsizetype len)
{
    Q_D(DataStream);
    QByteArray bytes(len, Qt::Uninitialized);
    const auto read = d->stream.readRawData(bytes.data(), len);
    if (read != len) {
        return notEnoughData(len, read);
    }
    return bytes;
}

template<>
DataStream::Result<QString> DataStream::read<QString>(qsizetype len)
{
    const auto bytes = read<QByteArray>(len);
    if (!bytes.has_value()) {
        return errorMsg(bytes.error());
    }
    return QString::fromUtf8(bytes.value());
}

template<std::integral T>
DataStream::Result<T> streamReadIntegral(DataStream &stream)
{
    const auto ret = stream.read<QByteArray>(sizeof(T));
    if (!ret.has_value()) {
        return std::unexpected(ret.error());
    }

    const auto &bytes = ret.value();
    T value = 0;
    if (stream.byteOrder() == DataStream::LittleEndian) {
        for (size_t i = 0; i < sizeof(T); ++i) {
            value |= static_cast<T>(static_cast<quint8>(bytes[i])) << (8 * i);
        }
    } else {
        for (size_t i = 0; i < sizeof(T); ++i) {
            value |= static_cast<T>(static_cast<quint8>(bytes[i])) << (8 * (sizeof(T) - i - 1));
        }
    }
    return value;
}

template<>
DataStream::Result<qint8> DataStream::read<qint8>()
{
    return streamReadIntegral<qint8>(*this);
}

template<>
DataStream::Result<quint8> DataStream::read<quint8>()
{
    return streamReadIntegral<quint8>(*this);
}

template<>
DataStream::Result<qint16> DataStream::read<qint16>()
{
    return streamReadIntegral<qint16>(*this);
}

template<>
DataStream::Result<quint16> DataStream::read<quint16>()
{
    return streamReadIntegral<quint16>(*this);
}

template<>
DataStream::Result<qint32> DataStream::read<qint32>()
{
    return streamReadIntegral<qint32>(*this);
}

template<>
DataStream::Result<quint32> DataStream::read<quint32>()
{
    return streamReadIntegral<quint32>(*this);
}

template<>
DataStream::Result<qint64> DataStream::read<qint64>()
{
    return streamReadIntegral<qint64>(*this);
}

template<>
DataStream::Result<quint64> DataStream::read<quint64>()
{
    return streamReadIntegral<quint64>(*this);
}

#ifdef QT_SUPPORTS_INT128
template<>
DataStream::Result<qint128> DataStream::read<qint128>()
{
    return streamReadIntegral<qint128>(*this);
}

template<>
DataStream::Result<quint128> DataStream::read<quint128>()
{
    return streamReadIntegral<quint128>(*this);
}
#endif

} // namespace MEASE
