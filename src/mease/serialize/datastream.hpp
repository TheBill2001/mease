#ifndef MEASE_DATASTREAM_HPP
#define MEASE_DATASTREAM_HPP

#include <QByteArray>
#include <QDataStream>
#include <QString>

namespace MEASE
{

class SaveFileHeaderData;

class DataStream : public QIODeviceBase
{
public:
    enum ByteOrder {
        BigEndian = QDataStream::BigEndian,
        LittleEndian = QDataStream::LittleEndian
    };

    struct Error {
        enum Type {
            NoError = 0,
            NotEnoughData,
            ChecksumMismatch
        };

        Type error = NoError;
        QString errorString;

        static Error notEnoughData(int expected, int got);
    };

    explicit DataStream(QIODevice *device);
    explicit DataStream(const QByteArray &bytes);
    explicit DataStream(QByteArray *array, QIODeviceBase::OpenMode mode);
    ~DataStream() = default;

    ByteOrder byteOrder() const;
    void setByteOrder(ByteOrder byteOrder);

    template<typename T>
    T read(Error *p_error = nullptr) = delete;

    template<typename T>
    T read(qsizetype len, Error *p_error = nullptr) = delete;

    template<std::integral T>
    T read(Error *p_error = nullptr)
    {
        Error error;
        const auto bytes = read<QByteArray>(sizeof(T), &error);
        if (p_error) {
            *p_error = error;
        }

        if (error.error != Error::NoError) {
            return 0;
        }

        T value = 0;
        if (byteOrder() == LittleEndian) {
            for (size_t i = 0; i < sizeof(T); ++i) {
                value |= static_cast<T>(bytes[i]) << (8 * i);
            }
        } else {
            for (size_t i = 0; i < sizeof(T); ++i) {
                value |= static_cast<T>(bytes[i]) << (8 * (sizeof(T) - i - 1));
            }
        }
        return value;
    }

    template<>
    QByteArray read<QByteArray>(qsizetype len, Error *p_error);

    template<>
    SaveFileHeaderData read<SaveFileHeaderData>(qsizetype len, Error *p_error);

private:
    QDataStream m_stream;
};

}

#endif // MEASE_DATASTREAM_HPP
