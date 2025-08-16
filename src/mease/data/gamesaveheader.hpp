// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_GAMESAVEHEADER_HPP
#define MEASE_GAMESAVEHEADER_HPP

#include <QDateTime>
#include <QMutex>

namespace MEASE
{
class GameSaveHeader
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
    Q_PROPERTY(quint64 dateTime MEMBER dateTime FINAL)
    Q_PROPERTY(QString levelTitleId MEMBER levelTitleId FINAL)
    Q_PROPERTY(QString levelFloorId MEMBER levelFloorId FINAL)
    Q_PROPERTY(QString levelRegionId MEMBER levelRegionId FINAL)
    Q_PROPERTY(quint64 totalPlaytime MEMBER totalPlaytime FINAL)
    Q_PROPERTY(QString nameOverrideStringId MEMBER nameOverrideStringId FINAL)
    Q_PROPERTY(QHash<quint32, QString> unknownValues MEMBER unknownValues FINAL)
public:
    quint16 version;

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
    quint64 dateTime;
    QString levelTitleId;
    QString levelFloorId;
    QString levelRegionId;
    quint64 totalPlaytime = 0;
    QString nameOverrideStringId;

    QHash<quint32, QString> unknownValues;

    bool operator==(const GameSaveHeader &other) const = default;
};

class GameSaveObject;
class GameSaveHeaderObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint16 version READ version CONSTANT FINAL)
    Q_PROPERTY(QString areaNameStringId WRITE setAreaNameStringId READ areaNameStringId NOTIFY areaNameStringIdChanged FINAL)
    Q_PROPERTY(QString areaThumbnailTextureId WRITE setAreaThumbnailTextureId READ areaThumbnailTextureId NOTIFY areaThumbnailTextureIdChanged FINAL)
    Q_PROPERTY(QString gameVersion WRITE setGameVersion READ gameVersion NOTIFY gameVersionChanged FINAL)
    Q_PROPERTY(QString requiredDLC WRITE setRequiredDLC READ requiredDLC NOTIFY requiredDLCChanged FINAL)
    Q_PROPERTY(QString requiredInstallGroup WRITE setRequiredInstallGroup READ requiredInstallGroup NOTIFY requiredInstallGroupChanged FINAL)
    Q_PROPERTY(QString profileName WRITE setProfileName READ profileName NOTIFY profileNameChanged FINAL)
    Q_PROPERTY(QString profileUniqueName WRITE setProfileUniqueName READ profileUniqueName NOTIFY profileUniqueNameChanged FINAL)
    Q_PROPERTY(QString profileId WRITE setProfileId READ profileId NOTIFY profileIdChanged FINAL)
    Q_PROPERTY(QString levelId WRITE setLevelId READ levelId NOTIFY levelIdChanged FINAL)
    Q_PROPERTY(QString playerLevel WRITE setPlayerLevel READ playerLevel NOTIFY playerLevelChanged FINAL)
    Q_PROPERTY(bool gameCompleted WRITE setGameCompleted READ gameCompleted NOTIFY gameCompletedChanged FINAL)
    Q_PROPERTY(bool trialMode WRITE setTrialMode READ trialMode NOTIFY trialModeChanged FINAL)
    Q_PROPERTY(double completionPercentage WRITE setCompletionPercentage READ completionPercentage NOTIFY completionPercentageChanged FINAL)
    Q_PROPERTY(quint64 dateTime WRITE setDateTime READ dateTime NOTIFY dateTimeChanged FINAL)
    Q_PROPERTY(QString levelTitleId WRITE setLevelTitleId READ levelTitleId NOTIFY levelTitleIdChanged FINAL)
    Q_PROPERTY(QString levelFloorId WRITE setLevelFloorId READ levelFloorId NOTIFY levelFloorIdChanged FINAL)
    Q_PROPERTY(QString levelRegionId WRITE setLevelRegionId READ levelRegionId NOTIFY levelRegionIdChanged FINAL)
    Q_PROPERTY(quint64 totalPlaytime WRITE setTotalPlaytime READ totalPlaytime NOTIFY totalPlaytimeChanged FINAL)
    Q_PROPERTY(QString nameOverrideStringId WRITE setNameOverrideStringId READ nameOverrideStringId NOTIFY nameOverrideStringIdChanged FINAL)
    Q_PROPERTY(QHash<quint32, QString> unknownValues WRITE setUnknownValues READ unknownValues NOTIFY unknownValuesChanged FINAL)

    friend GameSaveObject;
    GameSaveHeaderObject(GameSaveHeader &data, QMutex &mutex, GameSaveObject *parent);

public:
    quint16 version() const;

    void setAreaNameStringId(const QString &value);
    const QString &areaNameStringId() const;

    void setAreaThumbnailTextureId(const QString &value);
    const QString &areaThumbnailTextureId() const;

    void setGameVersion(const QString &value);
    const QString &gameVersion() const;

    void setRequiredDLC(const QString &value);
    const QString &requiredDLC() const;

    void setRequiredInstallGroup(const QString &value);
    const QString &requiredInstallGroup() const;

    void setProfileName(const QString &value);
    const QString &profileName() const;

    void setProfileUniqueName(const QString &value);
    const QString &profileUniqueName() const;

    void setProfileId(const QString &value);
    const QString &profileId() const;

    void setLevelId(const QString &value);
    const QString &levelId() const;

    void setPlayerLevel(const QString &value);
    const QString &playerLevel() const;

    void setGameCompleted(bool value);
    bool gameCompleted() const;

    void setTrialMode(bool value);
    bool trialMode() const;

    void setCompletionPercentage(double value);
    double completionPercentage() const;

    void setDateTime(quint64 value);
    quint64 dateTime() const;

    void setLevelTitleId(const QString &value);
    const QString &levelTitleId() const;

    void setLevelFloorId(const QString &value);
    const QString &levelFloorId() const;

    void setLevelRegionId(const QString &value);
    const QString &levelRegionId() const;

    void setTotalPlaytime(const quint64 &value);
    const quint64 &totalPlaytime() const;

    void setNameOverrideStringId(const QString &value);
    const QString &nameOverrideStringId() const;

    void setUnknownValues(const QHash<quint32, QString> &hash);
    const QHash<quint32, QString> &unknownValues() const;

    bool hasUnknownValue(quint32 key);
    QString getUnknownValue(quint32 key) const;
    void setUnknownValue(quint32 key, const QString &value);
    void clearUnknownValue(quint32 key);

Q_SIGNALS:
    void areaNameStringIdChanged(const QString &);
    void areaThumbnailTextureIdChanged(const QString &);
    void gameVersionChanged(const QString &);
    void requiredDLCChanged(const QString &);
    void requiredInstallGroupChanged(const QString &);
    void profileNameChanged(const QString &);
    void profileUniqueNameChanged(const QString &);
    void profileIdChanged(const QString &);
    void levelIdChanged(const QString &);
    void playerLevelChanged(const QString &);
    void gameCompletedChanged(bool);
    void trialModeChanged(bool);
    void completionPercentageChanged(double);
    void dateTimeChanged(quint64);
    void levelTitleIdChanged(const QString &);
    void levelFloorIdChanged(const QString &);
    void levelRegionIdChanged(const QString &);
    void totalPlaytimeChanged(const quint64 &);
    void nameOverrideStringIdChanged(const QString &);
    void unknownValuesChanged(const QHash<quint32, QString> &);

private:
    GameSaveHeader &m_data;
    QMutex &m_mutex;
};
} // namespace MEASE

Q_DECLARE_METATYPE(MEASE::GameSaveHeader)

#endif // MEASE_GAMESAVEHEADER_HPP
