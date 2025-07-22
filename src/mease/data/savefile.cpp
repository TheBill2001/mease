#include "mease/data/savefile.hpp"

namespace MEASE
{

SaveFile::SaveFile(const SaveFileData &data, QObject *parent)
    : QObject{parent}
    , m_data{data}
    , m_header{new SaveFileHeader(m_data.header, this)}
{
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