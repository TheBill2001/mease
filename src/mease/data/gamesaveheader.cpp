// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "gamesaveheader.hpp"

#include "mease/data/gamesave.hpp"
#include "mease/serialize/serializer_p.hpp"
#include "mease/utils/hashing/djb2.hpp"
#include "mease/utils/number.hpp"

#include <zlib.h>

#include <KLocalizedString>

using namespace Qt::StringLiterals;

namespace MEASE
{
namespace
{
constexpr quint64 signature_be = 0x4642484541444552; // "FBHEADER" in big endian
constexpr quint64 signature_le = swapEndian(signature_be);

enum Key {
    AreaNameStringId = modifiedDjbHash("AreaNameStringId"),
    AreaThumbnailTextureId = modifiedDjbHash("AreaThumbnailTextureId"),
    GameVersion = modifiedDjbHash("GameVersion"),
    RequiredDLC = modifiedDjbHash("RequiredDLC"),
    RequiredInstallGroup = modifiedDjbHash("RequiredInstallGroup"),
    ProfileName = modifiedDjbHash("ProfileName"),
    ProfileUniqueName = modifiedDjbHash("ProfileUniqueName"),
    ProfileId = modifiedDjbHash("ProfileId"),
    LevelID = modifiedDjbHash("LevelID"),
    PlayerLevel = modifiedDjbHash("PlayerLevel"),
    GameCompleted = modifiedDjbHash("GameCompleted"),
    TrialMode = modifiedDjbHash("TrialMode"),
    CompletionPercentage = modifiedDjbHash("CompletionPercentage"),
    DateTime = modifiedDjbHash("DateTime"),
    LevelTitleID = modifiedDjbHash("LevelTitleID"),
    LevelFloorID = modifiedDjbHash("LevelFloorID"),
    LevelRegionID = modifiedDjbHash("LevelRegionID"),
    TotalPlaytime = modifiedDjbHash("TotalPlaytime"),
    NameOverrideStringId = modifiedDjbHash("NameOverrideStringId"),
};
}

template<>
SerializerResult<GameSaveHeader> Serializer::read<GameSaveHeader>(qint64 length)
{
    QByteArray data;

    {
        READ(quint32, hash)
        READ_LENGTH_INTO(QByteArray, data, length - 4)

        const quint32 computedHash = crc32(crc2Seed, reinterpret_cast<const Bytef *>(data.constData()), data.size());
        if (hash != computedHash) {
            return std::unexpected(i18n("Mismatched header checksum."));
        }
    }

    Serializer serializer(data);
    serializer.setByteOrder(byteOrder());

    {
        SRLZR_READ(quint64, magic, serializer)
        bool failed = false;

        switch (serializer.byteOrder()) {
        case BigEndian:
            failed = magic != signature_be;
            break;
        case LittleEndian:
            failed = magic != signature_le;
            break;
        }

        if (failed) {
            return std::unexpected(i18n("Invalid metadata signature."));
        }
    }

    GameSaveHeader header;

    SRLZR_READ_INTO(quint16, header.version, serializer)

    if (header.version != 1) {
        return std::unexpected(i18n("Unsupported metadata version."));
    }

    SRLZR_READ(quint32, count, serializer)

    for (quint32 i = 0; i < count; ++i) {
        SRLZR_READ(quint32, key, serializer)
        SRLZR_READ(quint16, size, serializer)
        SRLZR_READ_LENGTH(QString, value, serializer, size)

        switch (key) {
        case AreaNameStringId:
            header.areaNameStringId = value;
            break;
        case AreaThumbnailTextureId:
            header.areaThumbnailTextureId = value;
            break;
        case GameVersion:
            header.gameVersion = value;
            break;
        case RequiredDLC:
            header.requiredDLC = value;
            break;
        case RequiredInstallGroup:
            header.requiredInstallGroup = value;
            break;
        case ProfileName:
            header.profileName = value;
            break;
        case ProfileUniqueName:
            header.profileUniqueName = value;
            break;
        case ProfileId:
            header.profileId = value;
            break;
        case LevelID:
            header.levelId = value;
            break;
        case PlayerLevel:
            header.playerLevel = value;
            break;
        case GameCompleted:
            header.gameCompleted = value == "true"_L1;
            break;
        case TrialMode:
            header.trialMode = value == "true"_L1;
            break;
        case CompletionPercentage: {
            bool ok = false;
            header.completionPercentage = value.toDouble(&ok);
            if (!ok) {
                return std::unexpected(i18n("Failed to read completion percentage: %1.", invalidFloatNumber.toString()));
            }
        } break;
        case DateTime: {
            bool ok = false;
            header.dateTime = value.toULongLong(&ok);
            if (!ok) {
                return std::unexpected(i18n("Failed to read date time: %1.", invalidIntegralNumber.toString()));
            }
        } break;
        case LevelTitleID:
            header.levelTitleId = value;
            break;
        case LevelFloorID:
            header.levelFloorId = value;
            break;
        case LevelRegionID:
            header.levelRegionId = value;
            break;
        case TotalPlaytime: {
            bool ok = false;
            header.totalPlaytime = value.toDouble(&ok) * 1000;
            if (!ok) {
                return std::unexpected(i18n("Failed to read total playtime: %1.", invalidFloatNumber.toString()));
            }
        } break;
        case NameOverrideStringId:
            header.nameOverrideStringId = value;
            break;
        default:
            header.unknownValues.emplace(key, value);
            break;
        }
    }

    return header;
}

GameSaveHeaderObject::GameSaveHeaderObject(GameSaveHeader &data, QMutex &mutex, GameSaveObject *parent)
    : QObject{parent}
    , m_data{data}
    , m_mutex{mutex}
{
}

quint16 GameSaveHeaderObject::version() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.version;
}

void GameSaveHeaderObject::setAreaNameStringId(const QString &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.areaNameStringId == value) {
            return;
        }
        m_data.areaNameStringId = value;
    }
    Q_EMIT areaNameStringIdChanged(value);
}

const QString &GameSaveHeaderObject::areaNameStringId() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.areaNameStringId;
}

void GameSaveHeaderObject::setAreaThumbnailTextureId(const QString &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.areaThumbnailTextureId == value) {
            return;
        }
        m_data.areaThumbnailTextureId = value;
    }
    Q_EMIT areaThumbnailTextureIdChanged(value);
}

const QString &GameSaveHeaderObject::areaThumbnailTextureId() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.areaThumbnailTextureId;
}

void GameSaveHeaderObject::setGameVersion(const QString &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.gameVersion == value) {
            return;
        }
        m_data.gameVersion = value;
    }
    Q_EMIT gameVersionChanged(value);
}

const QString &GameSaveHeaderObject::gameVersion() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.gameVersion;
}

void GameSaveHeaderObject::setRequiredDLC(const QString &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.requiredDLC == value) {
            return;
        }
        m_data.requiredDLC = value;
    }
    Q_EMIT requiredDLCChanged(value);
}

const QString &GameSaveHeaderObject::requiredDLC() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.requiredDLC;
}

void GameSaveHeaderObject::setRequiredInstallGroup(const QString &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.requiredInstallGroup == value) {
            return;
        }
        m_data.requiredInstallGroup = value;
    }
    Q_EMIT requiredInstallGroupChanged(value);
}

const QString &GameSaveHeaderObject::requiredInstallGroup() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.requiredInstallGroup;
}

void GameSaveHeaderObject::setProfileName(const QString &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.profileName == value) {
            return;
        }
        m_data.profileName = value;
    }
    Q_EMIT profileNameChanged(value);
}

const QString &GameSaveHeaderObject::profileName() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.profileName;
}

void GameSaveHeaderObject::setProfileUniqueName(const QString &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.profileUniqueName == value) {
            return;
        }
        m_data.profileUniqueName = value;
    }
    Q_EMIT profileUniqueNameChanged(value);
}

const QString &GameSaveHeaderObject::profileUniqueName() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.profileUniqueName;
}

void GameSaveHeaderObject::setProfileId(const QString &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.profileId == value) {
            return;
        }
        m_data.profileId = value;
    }
    Q_EMIT profileIdChanged(value);
}

const QString &GameSaveHeaderObject::profileId() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.profileId;
}

void GameSaveHeaderObject::setLevelId(const QString &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.levelId == value) {
            return;
        }
        m_data.levelId = value;
    }
    Q_EMIT levelIdChanged(value);
}

const QString &GameSaveHeaderObject::levelId() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.levelId;
}

void GameSaveHeaderObject::setPlayerLevel(const QString &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.playerLevel == value) {
            return;
        }
        m_data.playerLevel = value;
    }
    Q_EMIT playerLevelChanged(value);
}

const QString &GameSaveHeaderObject::playerLevel() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.playerLevel;
}

void GameSaveHeaderObject::setGameCompleted(bool value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.gameCompleted == value) {
            return;
        }
        m_data.gameCompleted = value;
    }
    Q_EMIT gameCompletedChanged(value);
}

bool GameSaveHeaderObject::gameCompleted() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.gameCompleted;
}

void GameSaveHeaderObject::setTrialMode(bool value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.trialMode == value) {
            return;
        }
        m_data.trialMode = value;
    }
    Q_EMIT trialModeChanged(value);
}

bool GameSaveHeaderObject::trialMode() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.trialMode;
}

void GameSaveHeaderObject::setCompletionPercentage(double value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.completionPercentage == value) {
            return;
        }
        m_data.completionPercentage = value;
    }
    Q_EMIT completionPercentageChanged(value);
}

double GameSaveHeaderObject::completionPercentage() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.completionPercentage;
}

void GameSaveHeaderObject::setDateTime(quint64 value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.dateTime == value) {
            return;
        }
        m_data.dateTime = value;
    }
    Q_EMIT dateTimeChanged(value);
}

quint64 GameSaveHeaderObject::dateTime() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.dateTime;
}

void GameSaveHeaderObject::setLevelTitleId(const QString &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.levelTitleId == value) {
            return;
        }
        m_data.levelTitleId = value;
    }
    Q_EMIT levelTitleIdChanged(value);
}

const QString &GameSaveHeaderObject::levelTitleId() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.levelTitleId;
}

void GameSaveHeaderObject::setLevelFloorId(const QString &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.levelFloorId == value) {
            return;
        }
        m_data.levelFloorId = value;
    }
    Q_EMIT levelFloorIdChanged(value);
}

const QString &GameSaveHeaderObject::levelFloorId() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.levelFloorId;
}

void GameSaveHeaderObject::setLevelRegionId(const QString &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.levelRegionId == value) {
            return;
        }
        m_data.levelRegionId = value;
    }
    Q_EMIT levelRegionIdChanged(value);
}

const QString &GameSaveHeaderObject::levelRegionId() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.levelRegionId;
}

void GameSaveHeaderObject::setTotalPlaytime(const quint64 &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.totalPlaytime == value) {
            return;
        }
        m_data.totalPlaytime = value;
    }
    Q_EMIT totalPlaytimeChanged(value);
}

const quint64 &GameSaveHeaderObject::totalPlaytime() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.totalPlaytime;
}

void GameSaveHeaderObject::setNameOverrideStringId(const QString &value)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.nameOverrideStringId == value) {
            return;
        }
        m_data.nameOverrideStringId = value;
    }
    Q_EMIT nameOverrideStringIdChanged(value);
}

const QString &GameSaveHeaderObject::nameOverrideStringId() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.nameOverrideStringId;
}

void GameSaveHeaderObject::setUnknownValues(const QHash<quint32, QString> &hash)
{
    {
        QMutexLocker lock(&m_mutex);
        if (m_data.unknownValues == hash) {
            return;
        }
        m_data.unknownValues = hash;
    }
    Q_EMIT unknownValuesChanged(hash);
}

const QHash<quint32, QString> &GameSaveHeaderObject::unknownValues() const
{
    QMutexLocker lock(&m_mutex);
    return m_data.unknownValues;
}

bool GameSaveHeaderObject::hasUnknownValue(quint32 key)
{
    QMutexLocker lock(&m_mutex);
    return m_data.unknownValues.contains(key);
}

QString GameSaveHeaderObject::getUnknownValue(quint32 key) const
{
    QMutexLocker lock(&m_mutex);
    return m_data.unknownValues.value(key);
}

void GameSaveHeaderObject::setUnknownValue(quint32 key, const QString &value)
{
    auto hash = unknownValues();
    if (hash[key] == value) {
        return;
    }
    setUnknownValues(hash);
}

void GameSaveHeaderObject::clearUnknownValue(quint32 key)
{
    auto hash = unknownValues();
    if (!hash.contains(key)) {
        return;
    }
    hash.remove(key);
    setUnknownValues(hash);
}
} // namespace MEASE
