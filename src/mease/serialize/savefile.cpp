#include "mease/data/savefile.hpp"
#include "mease/misc/number/endian.hpp"
#include "mease/serialize/datastream_p.hpp"

namespace MEASE
{

namespace
{

constexpr uint64_t signature_be = 0x46424348554E4B53; // "FBCHUNKS" in big endian
constexpr uint64_t signature_le = swapEndian(signature_be);

}

template<>
DataStream::Result<SaveFileData> DataStream::read<SaveFileData>()
{
    Q_D(DataStream);
    if (d->stream.device()->size() < 18) {
        return errorMsg(QObject::tr("Failed to read save file"), QObject::tr("File is too small"));
    }

    SaveFileData saveFile;
    setByteOrder(LittleEndian);

    {
        STRM_READ_MSG(quint64, magic, QObject::tr("Failed to read save file"));
        if (magic == signature_be || magic == signature_le) {
            if (magic == signature_le) {
                saveFile.isBigEndian = false;
                setByteOrder(LittleEndian);
            } else {
                saveFile.isBigEndian = true;
                setByteOrder(BigEndian);
            }
        } else {
            return errorMsg(QObject::tr("Failed to read save file"), QObject::tr("Invalid signature"));
        }
    }

    STRM_READ_ASSIGN_MSG(quint16, saveFile.version, QObject::tr("Failed to read save file"))
    if (saveFile.version != 1) {
        return unsupportedVersion(QObject::tr("Failed to read save file"), 1, saveFile.version);
    }

    STRM_READ_MSG(quint32, headerSize, QObject::tr("Failed to read save file"))
    STRM_READ_MSG(quint32, dataSize, QObject::tr("Failed to read save file"))

    STRM_READ_LEN_ASSIGN(SaveFileHeaderData, saveFile.header, headerSize)

    return saveFile;
}

} // namespace MEASE