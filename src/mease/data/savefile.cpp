#include "mease/data/savefile.hpp"

namespace MEASE
{

SaveFile::SaveFile(const SaveFileData &data, QObject *parent)
    : QObject{parent}
    , m_data{data}
    , m_header{new SaveFileHeader(m_data.header, this)}
{
}

bool SaveFile::isBigEndian() const
{
    return m_data.isBigEndian;
}

void SaveFile::setBigEndian(bool value)
{
    if (m_data.isBigEndian == value) {
        return;
    }
    m_data.isBigEndian = value;
    Q_EMIT isBigEndianChanged(m_data.version);
}

quint16 SaveFile::version() const
{
    return m_data.version;
}

void SaveFile::setVersion(quint16 version)
{
    if (m_data.version == version) {
        return;
    }
    m_data.version = version;
    Q_EMIT versionChanged(m_data.version);
}

SaveFileHeader *SaveFile::header() const
{
    return m_header;
}

} // namespace MEASE