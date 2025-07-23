#include "mease/data/savefileheader.hpp"
#include "mease/misc/hashing/djb2.hpp"
#include "mease/misc/number/datetime.hpp"
#include "mease/misc/number/endian.hpp"
#include "mease/serialize/datastream_p.hpp"

#include <zlib.h>

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
DataStream::Result<SaveFileHeaderData> DataStream::read<SaveFileHeaderData>(qsizetype len)
{
    QByteArray data;

    {
        STRM_READ_MSG(quint32, hash, QObject::tr("Failed to read header checksum"))

        STRM_READ_LEN_ASSIGN_MSG(QByteArray, data, len - sizeof(quint32), QObject::tr("Failed to read header data"))

        const quint32 computedHash = crc32(0x12345678, reinterpret_cast<const Bytef *>(data.constData()), data.size());
        if (hash != computedHash) {
            return checksumMismatch(QObject::tr("Failed to verify header data"), hash, computedHash);
        }
    }

    DataStream stream(data);

    {
        STREAM_READ_MSG(stream, quint64, magic, QObject::tr("Failed to read header signature"))

        switch (stream.byteOrder()) {
        case BigEndian:
            if (magic != signature_be) {
                return invalidSignature(QObject::tr("Failed to read header data"), signature_le, magic);
            }
            break;
        case LittleEndian:
            if (magic != signature_le) {
                return invalidSignature(QObject::tr("Failed to read header data"), signature_be, magic);
            }
            break;
        }
    }

    SaveFileHeaderData header;

    STREAM_READ_ASSIGN_MSG(stream, quint16, header.version, QObject::tr("Failed to read header data"))
    if (header.version != 1) {
        return unsupportedVersion(QObject::tr("Failed to read save file"), 1, header.version);
    }

    STREAM_READ_MSG(stream, quint32, count, QObject::tr("Failed to read header data"))

    for (quint32 i = 0; i < count; ++i) {
        STREAM_READ_MSG(stream, quint32, keyHash, QObject::tr("Failed to read header data"))
        STREAM_READ_MSG(stream, quint16, valueSize, QObject::tr("Failed to read header data"))
        STREAM_READ_LEN_MSG(stream, QString, value, valueSize, QObject::tr("Failed to read header data"))

        switch (keyHash) {
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
                return errorMsg(QObject::tr("Failed to read completion percentage"), QObject::tr("Not a valid double floating point number"));
            }
        } break;
        case DateTime: {
            bool ok = false;
            const quint64 numValue = value.toULongLong(&ok);
            if (!ok) {
                return errorMsg(QObject::tr("Failed to read header date time"), QObject::tr("Not a valid integer"));
            }
            header.dateTime = fromJulianSeconds(numValue);
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
            header.totalPlaytime = value.toDouble(&ok);
            if (!ok) {
                return errorMsg(QObject::tr("Failed to read total playtime"), QObject::tr("Not a valid double floating point number"));
            }
        } break;
        case NameOverrideStringId:
            header.nameOverrideStringId = value;
            break;
        default:
            header.unknownValues.emplace(keyHash, value);
            break;
        }
    }

    return header;
}

} // namespace MEASE