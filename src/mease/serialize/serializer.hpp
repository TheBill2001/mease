#ifndef MEASE_SERIALIZER_HPP
#define MEASE_SERIALIZER_HPP

#include <expected>

#include <QIODeviceBase>
#include <QScopedPointer>
#include <QStack>
#include <QString>

class QIODevice;

namespace MEASE
{
struct SerializerError {
    enum Error {
    };

    qint64 offset;
    Error error;
    QString errorString;
};

template<typename T>
using SerializerResult = std::expected<T, QString>;

class SerializerPrivate;
class Serializer : public QIODeviceBase
{
public:
    enum ByteOrder {
        BigEndian,
        LittleEndian
    };

    explicit Serializer(QIODevice *d);
    explicit Serializer(const QByteArray &data);
    explicit Serializer(QByteArray *data, OpenMode mode);
    ~Serializer();

    ByteOrder byteOrder() const;
    void setByteOrder(ByteOrder value);

    template<typename T>
    SerializerResult<T> read() = delete;

    template<typename T>
    SerializerResult<T> read(qint64 length) = delete;

    template<std::integral T>
    SerializerResult<T> read();

private:
    Q_DECLARE_PRIVATE(Serializer)
    QScopedPointer<SerializerPrivate> d_ptr;
};

template<>
SerializerResult<QByteArray> Serializer::read<QByteArray>(qint64 length);

template<>
SerializerResult<QString> Serializer::read<QString>(qint64 length);

template<std::integral T>
SerializerResult<T> Serializer::read()
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

class GameSave;
template<>
SerializerResult<GameSave> Serializer::read<GameSave>();

class GameSaveHeader;
template<>
SerializerResult<GameSaveHeader> Serializer::read<GameSaveHeader>(qint64 length);
} // namespace MEASE

#endif // MEASE_SERIALIZER_HPP
