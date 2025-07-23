#ifndef MEASE_DATASTREAM_HPP
#define MEASE_DATASTREAM_HPP

#include <concepts>
#include <expected>

#include <QByteArray>
#include <QIODevice>
#include <QString>

namespace MEASE
{

class SaveFileHeaderData;
class SaveFileData;

class DataStreamPrivate;
class DataStream : public QIODeviceBase
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

    template<std::integral T>
    Result<T> read()
    {
        const auto ret = read<QByteArray>(sizeof(T));
        if (!ret.has_value()) {
            return std::unexpected(ret.error());
        }

        const auto &bytes = ret.value();
        T value = 0;
        if (byteOrder() == LittleEndian) {
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
    Result<QByteArray> read<QByteArray>(qsizetype len);

    template<>
    Result<QString> read<QString>(qsizetype len);

    template<>
    Result<SaveFileData> read<SaveFileData>();

    template<>
    Result<SaveFileHeaderData> read<SaveFileHeaderData>(qsizetype len);

private:
    Q_DECLARE_PRIVATE(DataStream)
    QScopedPointer<DataStreamPrivate> d_ptr;
};

}

#endif // MEASE_DATASTREAM_HPP
