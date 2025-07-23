#include "mease/serialize/datastream_p.hpp"

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

} // namespace MEASE
