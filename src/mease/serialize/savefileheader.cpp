#include "mease/data/savefileheader.hpp"
#include "mease/misc/hashing/djb2.hpp"
#include "mease/misc/number/endian.hpp"
#include "mease/serialize/datastream.hpp"
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
SaveFileHeaderData DataStream::read<SaveFileHeaderData>(qsizetype len, Error *p_error)
{
    Error error;
    if (p_error) {
        *p_error = error;
    }

    STREAM_RET_LEN(QByteArray, bytes, len, error, QObject::tr("Failed to read header"))

    DataStream stream(bytes);

    STREAM_RET(quint32, hash, error, QObject::tr("Failed to read header hash"))
    STREAM_RET_LEN(QByteArray, data, len - sizeof(quint32), error, QObject::tr("Failed to read header data"))

    quint32 computedHash = crc32(0x12345678, reinterpret_cast<const Bytef *>(data.constData()), data.size());
    STREAM_RET_COND(hash != computedHash,
                    p_error,
                    Error::ChecksumMismatch,
                    QObject::tr("Mismatched checksum for header data, expected %1, got %2")
                        .arg(QString::number(hash, 16).toUpper(), QString::number(computedHash, 16).toUpper()))

    return {};
}

} // namespace MEASE