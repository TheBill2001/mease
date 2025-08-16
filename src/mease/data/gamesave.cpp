// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "gamesave.hpp"

#include "mease/serialize/serializer_p.hpp"
#include "mease/utils/number.hpp"

#include <KLocalizedString>

using namespace Qt::StringLiterals;

namespace MEASE
{
namespace
{
constexpr uint64_t signature_be = 0x46424348554E4B53; // "FBCHUNKS" in big endian
constexpr uint64_t signature_le = swapEndian(signature_be);
}

template<>
SerializerResult<GameSave> Serializer::read<GameSave>()
{
    Q_D(Serializer);
    if (d->device->size() < 18) {
        return std::unexpected(i18n("File is too small."));
    }

    GameSave gameSave;
    setByteOrder(LittleEndian);

    {
        READ(quint64, magic)

        if (magic == signature_be || magic == signature_le) {
            if (magic == signature_le) {
                gameSave.isBigEndian = false;
                setByteOrder(LittleEndian);
            } else {
                gameSave.isBigEndian = true;
                setByteOrder(BigEndian);
            }
        } else {
            return std::unexpected(i18n("Invalid save file signature."));
        }
    }

    READ_INTO(quint16, gameSave.version)

    if (gameSave.version != 1) {
        return std::unexpected(i18n("Unsupported save file version."));
    }

    READ(quint32, headerSize)
    READ(quint32, dataSize)

    READ_LENGTH_INTO(GameSaveHeader, gameSave.header, headerSize)
    READ_LENGTH_INTO(QByteArray, gameSave.data, dataSize)

    return gameSave;
}

GameSaveObject::GameSaveObject(const GameSave &data, QObject *parent)
    : QObject{parent}
    , m_data{data}
    , m_metadata{new GameSaveHeaderObject(m_data.header, m_mutex, this)}
{
}

GameSaveObject::GameSaveObject(GameSave &&data, QObject *parent)
    : QObject{parent}
    , m_data{data}
    , m_metadata{new GameSaveHeaderObject(m_data.header, m_mutex, this)}
{
}

bool GameSaveObject::isBigEndian() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.isBigEndian;
}

void GameSaveObject::setBigEndian(bool value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.isBigEndian == value) {
            return;
        }
        m_data.isBigEndian = value;
    }
    Q_EMIT isBigEndianChanged(value);
}

const QUrl &GameSaveObject::filePath() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.filePath;
}

void GameSaveObject::setFilePath(const QUrl &url)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.filePath == url) {
            return;
        }
        m_data.filePath = url;
    }
    Q_EMIT filePathChanged(url);
}

quint16 GameSaveObject::version() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.version;
}

void GameSaveObject::setVersion(quint16 version)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.version == version) {
            return;
        }
        m_data.version = version;
    }
    Q_EMIT versionChanged(version);
}

GameSaveHeaderObject *GameSaveObject::header() const
{
    return m_metadata;
}
} // namespace MEASE
