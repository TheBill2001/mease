#ifndef MEASE_DATA_SAVEFILE_HPP
#define MEASE_DATA_SAVEFILE_HPP

#include "mease/data/savefileheader.hpp"

#include <QFileInfo>

namespace MEASE
{
class SaveFileData final
{
    Q_GADGET
    Q_PROPERTY(bool isBigEndian MEMBER isBigEndian FINAL)
    Q_PROPERTY(QFileInfo fileInfo MEMBER fileInfo FINAL)
    Q_PROPERTY(quint16 version MEMBER version FINAL)
    Q_PROPERTY(SaveFileHeaderData header MEMBER header FINAL)
public:
    bool isBigEndian = false;
    QFileInfo fileInfo;

    quint16 version = 0;
    SaveFileHeaderData header;

    bool operator==(const SaveFileData &other) const = default;
};

class SaveFile final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isBigEndian READ isBigEndian WRITE setBigEndian NOTIFY isBigEndianChanged FINAL)
    Q_PROPERTY(quint16 version READ version WRITE setVersion NOTIFY versionChanged FINAL)
    Q_PROPERTY(MEASE::SaveFileHeader *header READ header CONSTANT FINAL)
public:
    explicit SaveFile(const SaveFileData &data, QObject *parent = nullptr);

    bool isBigEndian() const;
    void setBigEndian(bool value);

    quint16 version() const;
    void setVersion(quint16 version);

    SaveFileHeader *header() const;

Q_SIGNALS:
    void isBigEndianChanged(bool value);
    void versionChanged(quint16 version);

private:
    SaveFileData m_data;
    SaveFileHeader *m_header;
};
} // namespace MEASE

Q_DECLARE_METATYPE(MEASE::SaveFileData)

#endif // MEASE_DATA_SAVEFILE_HPP
