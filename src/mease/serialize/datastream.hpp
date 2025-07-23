#ifndef MEASE_DATASTREAM_HPP
#define MEASE_DATASTREAM_HPP

#include <expected>

#include <QByteArray>
#include <QIODevice>
#include <QString>

namespace MEASE
{

class DataStreamPrivate;
class DataStream final : public QIODeviceBase
{
public:
    template<typename T>
    using Result = std::expected<T, QString>;

    enum ByteOrder {
        BigEndian = QDataStream::BigEndian,
        LittleEndian = QDataStream::LittleEndian
    };

    explicit DataStream(QIODevice *device);
    explicit DataStream(const QByteArray &bytes);
    explicit DataStream(QByteArray *array, QIODeviceBase::OpenMode mode);
    ~DataStream();

    ByteOrder byteOrder() const;
    void setByteOrder(ByteOrder byteOrder);

    template<typename T>
    Result<T> read() = delete;

    template<typename T>
    Result<T> read(qsizetype len) = delete;

private:
    Q_DECLARE_PRIVATE(DataStream)
    QScopedPointer<DataStreamPrivate> d_ptr;
};

template<>
DataStream::Result<qint8> DataStream::read<qint8>();

template<>
DataStream::Result<quint8> DataStream::read<quint8>();

template<>
DataStream::Result<qint16> DataStream::read<qint16>();

template<>
DataStream::Result<quint16> DataStream::read<quint16>();

template<>
DataStream::Result<qint32> DataStream::read<qint32>();

template<>
DataStream::Result<quint32> DataStream::read<quint32>();

template<>
DataStream::Result<qint64> DataStream::read<qint64>();

template<>
DataStream::Result<quint64> DataStream::read<quint64>();

#ifdef QT_SUPPORTS_INT128
template<>
DataStream::Result<qint128> DataStream::read<qint128>();

template<>
DataStream::Result<quint128> DataStream::read<quint128>();
#endif

template<>
DataStream::Result<QByteArray> DataStream::read<QByteArray>(qsizetype len);

template<>
DataStream::Result<QString> DataStream::read<QString>(qsizetype len);

class SaveFileData;
template<>
DataStream::Result<SaveFileData> DataStream::read<SaveFileData>();

class SaveFileHeaderData;
template<>
DataStream::Result<SaveFileHeaderData> DataStream::read<SaveFileHeaderData>(qsizetype len);
}

#endif // MEASE_DATASTREAM_HPP
