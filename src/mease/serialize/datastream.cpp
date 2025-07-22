#include "mease/serialize/datastream.hpp"

#include <QObject>

using namespace Qt::Literals::StringLiterals;

namespace MEASE
{

DataStream::Error DataStream::Error::notEnoughData(int expected, int got)
{
    return {NotEnoughData,
            u"%1, %2, %3"_s.arg(QObject::tr("Not enough data", "SaveFileStream::Error::NotEnoughData"),
                                QObject::tr("expected %n byte(s)", "SaveFileStream::Error::NotEnoughData", expected),
                                QObject::tr("got %n byte(s)", "SaveFileStream::Error::NotEnoughData", got))};
}

DataStream::DataStream(QIODevice *device)
    : m_stream{device}
{
}

DataStream::DataStream(const QByteArray &bytes)
    : m_stream{bytes}
{
}

DataStream::DataStream(QByteArray *array, QIODeviceBase::OpenMode mode)
    : m_stream{array, mode}
{
}

DataStream::ByteOrder DataStream::byteOrder() const
{
    return static_cast<DataStream::ByteOrder>(m_stream.byteOrder());
}

void DataStream::setByteOrder(ByteOrder byteOrder)
{
    m_stream.setByteOrder(static_cast<QDataStream::ByteOrder>(byteOrder));
}

template<>
QByteArray DataStream::read<QByteArray>(qsizetype len, Error *p_error)
{
    QByteArray bytes(len, Qt::Uninitialized);
    const auto read = m_stream.readRawData(bytes.data(), len);
    if (read != len) {
        bytes.resize(read);
        bytes.squeeze();
        *p_error = Error::notEnoughData(len, read);
    }
    return bytes;
}

} // namespace MEASE
