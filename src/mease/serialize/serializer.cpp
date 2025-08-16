#include "serializer.hpp"

#include "mease/serialize/serializer_p.hpp"
#include "mease/utils/number.hpp"

#include <QBuffer>

#include <KLocalizedString>

using namespace Qt::StringLiterals;

namespace MEASE
{
Serializer::Serializer(QIODevice *d)
    : d_ptr{new SerializerPrivate(d, this)}
{
}

Serializer::Serializer(const QByteArray &data)
    : d_ptr{new SerializerPrivate(new QBuffer, this)}
{
    Q_D(Serializer);
    auto *buffer = static_cast<QBuffer *>(d->device);
    buffer->setData(data);
    buffer->open(ReadOnly);
    d->ownDevice = true;
}

Serializer::Serializer(QByteArray *data, OpenMode mode)
    : d_ptr{new SerializerPrivate(new QBuffer(data), this)}
{
    Q_D(Serializer);
    d->device->open(mode);
    d->ownDevice = true;
}

Serializer::~Serializer() = default;

Serializer::ByteOrder Serializer::byteOrder() const
{
    Q_D(const Serializer);
    return d->byteOrder;
}

void Serializer::setByteOrder(ByteOrder value)
{
    Q_D(Serializer);
    d->byteOrder = value;
}

template<>
SerializerResult<QByteArray> Serializer::read(qint64 length)
{
    Q_D(Serializer);
    const auto pos = d->device->pos();
    QByteArray data(length, Qt::Uninitialized);
    const auto readLength = d->device->read(data.data(), length);
    if (readLength != length) {
        auto errorString = d->device->errorString();
        return std::unexpected(i18np("Failed to read %1 byte at %2: %3.",
                                     "Failed to read %1 bytes at %2: %3.",
                                     length,
                                     toHex(pos),
                                     (errorString.isEmpty() ? i18n("Not enough data") : errorString)));
    };
    return data;
}

template<>
SerializerResult<QString> Serializer::read<QString>(qint64 length)
{
    auto data = read<QByteArray>(length);
    if (data.has_value()) {
        return QString::fromUtf8(data.value());
    }
    return std::unexpected(std::move(data.error()));
}
} // namespace MEASE
