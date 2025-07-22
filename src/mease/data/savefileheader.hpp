#ifndef MEASE_DATA_SAVEFILEHEADER_HPP
#define MEASE_DATA_SAVEFILEHEADER_HPP

#include <QDateTime>
#include <QObject>

namespace MEASE
{

class SaveFileHeaderData
{
    Q_GADGET
    Q_PROPERTY(quint16 version MEMBER version FINAL)
    Q_PROPERTY(QString areaNameStringId MEMBER areaNameStringId FINAL)
    Q_PROPERTY(QString areaThumbnailTextureId MEMBER areaThumbnailTextureId FINAL)
    Q_PROPERTY(QString gameVersion MEMBER gameVersion FINAL)
    Q_PROPERTY(QString requiredDLC MEMBER requiredDLC FINAL)
    Q_PROPERTY(QString requiredInstallGroup MEMBER requiredInstallGroup FINAL)
    Q_PROPERTY(QString profileName MEMBER profileName FINAL)
    Q_PROPERTY(QString profileUniqueName MEMBER profileUniqueName FINAL)
    Q_PROPERTY(QString profileId MEMBER profileId FINAL)
    Q_PROPERTY(QString levelId MEMBER levelId FINAL)
    Q_PROPERTY(QString playerLevel MEMBER playerLevel FINAL)
    Q_PROPERTY(bool gameCompleted MEMBER gameCompleted FINAL)
    Q_PROPERTY(bool trialMode MEMBER trialMode FINAL)
    Q_PROPERTY(double completionPercentage MEMBER completionPercentage FINAL)
    Q_PROPERTY(QDateTime dateTime MEMBER dateTime FINAL)
    Q_PROPERTY(QString levelTitleId MEMBER levelTitleId FINAL)
    Q_PROPERTY(QString levelFloorId MEMBER levelFloorId FINAL)
    Q_PROPERTY(QString levelRegionId MEMBER levelRegionId FINAL)
    Q_PROPERTY(quint64 totalPlaytime MEMBER totalPlaytime FINAL)
    Q_PROPERTY(QString nameOverrideStringId MEMBER nameOverrideStringId FINAL)
    Q_PROPERTY(QHash<quint32, QString> unknownValues MEMBER unknownValues FINAL)
public:
    quint16 version = 0;
    QString areaNameStringId;
    QString areaThumbnailTextureId;
    QString gameVersion;
    QString requiredDLC;
    QString requiredInstallGroup;
    QString profileName;
    QString profileUniqueName;
    QString profileId;
    QString levelId;
    QString playerLevel;
    bool gameCompleted = false;
    bool trialMode = false;
    double completionPercentage = 0.0;
    QDateTime dateTime;
    QString levelTitleId;
    QString levelFloorId;
    QString levelRegionId;
    quint64 totalPlaytime = 0;
    QString nameOverrideStringId;

    QHash<quint32, QString> unknownValues;

    bool operator==(const SaveFileHeaderData &other) const = default;
};

class SaveFileHeader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint16 version READ version WRITE setVersion NOTIFY versionChanged FINAL)
    Q_PROPERTY(QString areaNameStringId READ areaNameStringId WRITE setAreaNameStringId NOTIFY areaNameStringIdChanged FINAL)
    Q_PROPERTY(QString areaThumbnailTextureId READ areaThumbnailTextureId WRITE setAreaThumbnailTextureId NOTIFY areaThumbnailTextureIdChanged FINAL)
    Q_PROPERTY(QString gameVersion READ gameVersion WRITE setGameVersion NOTIFY gameVersionChanged FINAL)
    Q_PROPERTY(QString requiredDLC READ requiredDLC WRITE setRequiredDLC NOTIFY requiredDLCChanged FINAL)
    Q_PROPERTY(QString requiredInstallGroup READ requiredInstallGroup WRITE setRequiredInstallGroup NOTIFY requiredInstallGroupChanged FINAL)
    Q_PROPERTY(QString profileName READ profileName WRITE setProfileName NOTIFY profileNameChanged FINAL)
    Q_PROPERTY(QString profileUniqueName READ profileUniqueName WRITE setProfileUniqueName NOTIFY profileUniqueNameChanged FINAL)
    Q_PROPERTY(QString profileId READ profileId WRITE setProfileId NOTIFY profileIdChanged FINAL)
    Q_PROPERTY(QString levelId READ levelId WRITE setLevelId NOTIFY levelIdChanged FINAL)
    Q_PROPERTY(QString playerLevel READ playerLevel WRITE setPlayerLevel NOTIFY playerLevelChanged FINAL)
    Q_PROPERTY(bool gameCompleted READ gameCompleted WRITE setGameCompleted NOTIFY gameCompletedChanged FINAL)
    Q_PROPERTY(bool trialMode READ trialMode WRITE setTrialMode NOTIFY trialModeChanged FINAL)
    Q_PROPERTY(double completionPercentage READ completionPercentage WRITE setCompletionPercentage NOTIFY completionPercentageChanged FINAL)
    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged FINAL)
    Q_PROPERTY(QString levelTitleId READ levelTitleId WRITE setLevelTitleId NOTIFY levelTitleIdChanged FINAL)
    Q_PROPERTY(QString levelFloorId READ levelFloorId WRITE setLevelFloorId NOTIFY levelFloorIdChanged FINAL)
    Q_PROPERTY(QString levelRegionId READ levelRegionId WRITE setLevelRegionId NOTIFY levelRegionIdChanged FINAL)
    Q_PROPERTY(quint64 totalPlaytime READ totalPlaytime WRITE setTotalPlaytime NOTIFY totalPlaytimeChanged FINAL)
    Q_PROPERTY(QString nameOverrideStringId READ nameOverrideStringId WRITE setNameOverrideStringId NOTIFY nameOverrideStringIdChanged FINAL)
    Q_PROPERTY(QHash<quint32, QString> unknownValues READ unknownValues WRITE setUnknownValues NOTIFY unknownValuesChanged FINAL)
private:
    friend class SaveFile;
    explicit SaveFileHeader(SaveFileHeaderData &data, QObject *parent = nullptr);

public:
    quint16 version() const;
    void setVersion(quint16 version);

    const QString &areaNameStringId() const;
    void setAreaNameStringId(const QString &areaNameStringId);

    const QString &areaThumbnailTextureId() const;
    void setAreaThumbnailTextureId(const QString &areaThumbnailTextureId);

    const QString &gameVersion() const;
    void setGameVersion(const QString &gameVersion);

    const QString &requiredDLC() const;
    void setRequiredDLC(const QString &requiredDLC);

    const QString &requiredInstallGroup() const;
    void setRequiredInstallGroup(const QString &requiredInstallGroup);

    const QString &profileName() const;
    void setProfileName(const QString &profileName);

    const QString &profileUniqueName() const;
    void setProfileUniqueName(const QString &profileUniqueName);

    const QString &profileId() const;
    void setProfileId(const QString &profileId);

    const QString &levelId() const;
    void setLevelId(const QString &levelId);

    const QString &playerLevel() const;
    void setPlayerLevel(const QString &playerLevel);

    bool gameCompleted() const;
    void setGameCompleted(bool gameCompleted);

    bool trialMode() const;
    void setTrialMode(bool trialMode);

    double completionPercentage() const;
    void setCompletionPercentage(double completionPercentage);

    QDateTime dateTime() const;
    void setDateTime(const QDateTime &dateTime);

    const QString &levelTitleId() const;
    void setLevelTitleId(const QString &levelTitleId);

    const QString &levelFloorId() const;
    void setLevelFloorId(const QString &levelFloorId);

    const QString &levelRegionId() const;
    void setLevelRegionId(const QString &levelRegionId);

    quint64 totalPlaytime() const;
    void setTotalPlaytime(const quint64 &totalPlaytime);

    const QString &nameOverrideStringId() const;
    void setNameOverrideStringId(const QString &nameOverrideStringId);

    const QHash<quint32, QString> &unknownValues() const;
    void setUnknownValues(const QHash<quint32, QString> &values);

    const QString &unknownValue(quint32 key) const;
    void setUnknownValue(quint32 key, const QString &value);

Q_SIGNALS:
    void versionChanged(quint16 version);
    void areaNameStringIdChanged(const QString &areaNameStringId);
    void areaThumbnailTextureIdChanged(const QString &areaThumbnailTextureId);
    void gameVersionChanged(const QString &gameVersion);
    void requiredDLCChanged(const QString &requiredDLC);
    void requiredInstallGroupChanged(const QString &requiredInstallGroup);
    void profileNameChanged(const QString &profileName);
    void profileUniqueNameChanged(const QString &profileUniqueName);
    void profileIdChanged(const QString &profileId);
    void levelIdChanged(const QString &levelId);
    void playerLevelChanged(const QString &playerLevel);
    void gameCompletedChanged(bool gameCompleted);
    void trialModeChanged(bool trialMode);
    void completionPercentageChanged(double completionPercentage);
    void dateTimeChanged(const QDateTime &dateTime);
    void levelTitleIdChanged(const QString &levelTitleId);
    void levelFloorIdChanged(const QString &levelFloorId);
    void levelRegionIdChanged(const QString &levelRegionId);
    void totalPlaytimeChanged(const quint64 &totalPlaytime);
    void nameOverrideStringIdChanged(const QString &nameOverrideStringId);
    void unknownValuesChanged(const QHash<quint32, QString> &unknownValues);
    void unknownValueChanged(quint32 key, const QString &value);

private:
    SaveFileHeaderData &m_data;
};

} // namespace MEASE

Q_DECLARE_METATYPE(MEASE::SaveFileHeaderData)

#endif // MEASE_DATA_SAVEFILEHEADER_HPP
