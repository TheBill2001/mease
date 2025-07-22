#ifndef MEASE_DATA_SAVEFILE_HPP
#define MEASE_DATA_SAVEFILE_HPP

#include "mease/data/savefileheader.hpp"

namespace MEASE
{

class SaveFileData
{
    Q_GADGET
    Q_PROPERTY(quint16 version MEMBER version FINAL)
    Q_PROPERTY(SaveFileHeaderData header MEMBER header FINAL)
public:
    quint16 version = 0;

    SaveFileHeaderData header;

    bool operator==(const SaveFileData &other) const = default;
};

class SaveFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint16 version READ version WRITE setVersion NOTIFY versionChanged FINAL)
private:
    explicit SaveFile(const SaveFileData &data, QObject *parent = nullptr);

public:
    quint16 version() const;
    void setVersion(quint16 version);

    SaveFileHeader *header() const;

Q_SIGNALS:
    void versionChanged(quint16 version);

private:
    SaveFileData m_data;

    SaveFileHeader *m_header;
};

} // namespace MEASE

Q_DECLARE_METATYPE(MEASE::SaveFileData)

#endif // MEASE_DATA_SAVEFILE_HPP
