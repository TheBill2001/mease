#include "mease/data/savefileheader.hpp"

namespace MEASE
{

SaveFileHeader::SaveFileHeader(SaveFileHeaderData &data, QObject *parent)
    : QObject{parent}
    , m_data{data}
{
}

quint16 SaveFileHeader::version() const
{
    return m_data.version;
}

void SaveFileHeader::setVersion(quint16 version)
{
    if (m_data.version == version) {
        return;
    }
    m_data.version = version;
    Q_EMIT versionChanged(m_data.version);
}

const QString &SaveFileHeader::areaNameStringId() const
{
    return m_data.areaNameStringId;
}

void SaveFileHeader::setAreaNameStringId(const QString &areaNameStringId)
{
    if (m_data.areaNameStringId == areaNameStringId) {
        return;
    }
    m_data.areaNameStringId = areaNameStringId;
    Q_EMIT areaNameStringIdChanged(m_data.areaNameStringId);
}

const QString &SaveFileHeader::areaThumbnailTextureId() const
{
    return m_data.areaThumbnailTextureId;
}

void SaveFileHeader::setAreaThumbnailTextureId(const QString &areaThumbnailTextureId)
{
    if (m_data.areaThumbnailTextureId == areaThumbnailTextureId) {
        return;
    }
    m_data.areaThumbnailTextureId = areaThumbnailTextureId;
    Q_EMIT areaThumbnailTextureIdChanged(m_data.areaThumbnailTextureId);
}

const QString &SaveFileHeader::gameVersion() const
{
    return m_data.gameVersion;
}

void SaveFileHeader::setGameVersion(const QString &gameVersion)
{
    if (m_data.gameVersion == gameVersion) {
        return;
    }
    m_data.gameVersion = gameVersion;
    Q_EMIT gameVersionChanged(m_data.gameVersion);
}

const QString &SaveFileHeader::requiredDLC() const
{
    return m_data.requiredDLC;
}

void SaveFileHeader::setRequiredDLC(const QString &requiredDLC)
{
    if (m_data.requiredDLC == requiredDLC) {
        return;
    }
    m_data.requiredDLC = requiredDLC;
    Q_EMIT requiredDLCChanged(m_data.requiredDLC);
}

const QString &SaveFileHeader::requiredInstallGroup() const
{
    return m_data.requiredInstallGroup;
}

void SaveFileHeader::setRequiredInstallGroup(const QString &requiredInstallGroup)
{
    if (m_data.requiredInstallGroup == requiredInstallGroup) {
        return;
    }
    m_data.requiredInstallGroup = requiredInstallGroup;
    Q_EMIT requiredInstallGroupChanged(m_data.requiredInstallGroup);
}

const QString &SaveFileHeader::profileName() const
{
    return m_data.profileName;
}

void SaveFileHeader::setProfileName(const QString &profileName)
{
    if (m_data.profileName == profileName) {
        return;
    }
    m_data.profileName = profileName;
    Q_EMIT profileNameChanged(m_data.profileName);
}

const QString &SaveFileHeader::profileUniqueName() const
{
    return m_data.profileUniqueName;
}

void SaveFileHeader::setProfileUniqueName(const QString &profileUniqueName)
{
    if (m_data.profileUniqueName == profileUniqueName) {
        return;
    }
    m_data.profileUniqueName = profileUniqueName;
    Q_EMIT profileUniqueNameChanged(m_data.profileUniqueName);
}

const QString &SaveFileHeader::profileId() const
{
    return m_data.profileId;
}

void SaveFileHeader::setProfileId(const QString &profileId)
{
    if (m_data.profileId == profileId) {
        return;
    }
    m_data.profileId = profileId;
    Q_EMIT profileIdChanged(m_data.profileId);
}

const QString &SaveFileHeader::levelId() const
{
    return m_data.levelId;
}

void SaveFileHeader::setLevelId(const QString &levelId)
{
    if (m_data.levelId == levelId) {
        return;
    }
    m_data.levelId = levelId;
    Q_EMIT levelIdChanged(m_data.levelId);
}

const QString &SaveFileHeader::playerLevel() const
{
    return m_data.playerLevel;
}

void SaveFileHeader::setPlayerLevel(const QString &playerLevel)
{
    if (m_data.playerLevel == playerLevel) {
        return;
    }
    m_data.playerLevel = playerLevel;
    Q_EMIT playerLevelChanged(m_data.playerLevel);
}

bool SaveFileHeader::gameCompleted() const
{
    return m_data.gameCompleted;
}

void SaveFileHeader::setGameCompleted(bool gameCompleted)
{
    if (m_data.gameCompleted == gameCompleted) {
        return;
    }
    m_data.gameCompleted = gameCompleted;
    Q_EMIT gameCompletedChanged(m_data.gameCompleted);
}

bool SaveFileHeader::trialMode() const
{
    return m_data.trialMode;
}

void SaveFileHeader::setTrialMode(bool trialMode)
{
    if (m_data.trialMode == trialMode) {
        return;
    }
    m_data.trialMode = trialMode;
    Q_EMIT trialModeChanged(m_data.trialMode);
}

double SaveFileHeader::completionPercentage() const
{
    return m_data.completionPercentage;
}

void SaveFileHeader::setCompletionPercentage(double completionPercentage)
{
    if (m_data.completionPercentage == completionPercentage) {
        return;
    }
    m_data.completionPercentage = completionPercentage;
    Q_EMIT completionPercentageChanged(m_data.completionPercentage);
}

QDateTime SaveFileHeader::dateTime() const
{
    return m_data.dateTime;
}

void SaveFileHeader::setDateTime(const QDateTime &dateTime)
{
    if (m_data.dateTime == dateTime) {
        return;
    }
    m_data.dateTime = dateTime;
    Q_EMIT dateTimeChanged(m_data.dateTime);
}

const QString &SaveFileHeader::levelTitleId() const
{
    return m_data.levelTitleId;
}

void SaveFileHeader::setLevelTitleId(const QString &levelTitleId)
{
    if (m_data.levelTitleId == levelTitleId) {
        return;
    }
    m_data.levelTitleId = levelTitleId;
    Q_EMIT levelTitleIdChanged(m_data.levelTitleId);
}

const QString &SaveFileHeader::levelFloorId() const
{
    return m_data.levelFloorId;
}

void SaveFileHeader::setLevelFloorId(const QString &levelFloorId)
{
    if (m_data.levelFloorId == levelFloorId) {
        return;
    }
    m_data.levelFloorId = levelFloorId;
    Q_EMIT levelFloorIdChanged(m_data.levelFloorId);
}

const QString &SaveFileHeader::levelRegionId() const
{
    return m_data.levelRegionId;
}

void SaveFileHeader::setLevelRegionId(const QString &levelRegionId)
{
    if (m_data.levelRegionId == levelRegionId) {
        return;
    }
    m_data.levelRegionId = levelRegionId;
    Q_EMIT levelRegionIdChanged(m_data.levelRegionId);
}

quint64 SaveFileHeader::totalPlaytime() const
{
    return m_data.totalPlaytime;
}

void SaveFileHeader::setTotalPlaytime(const quint64 &totalPlaytime)
{
    if (m_data.totalPlaytime == totalPlaytime) {
        return;
    }
    m_data.totalPlaytime = totalPlaytime;
    Q_EMIT totalPlaytimeChanged(m_data.totalPlaytime);
}

const QString &SaveFileHeader::nameOverrideStringId() const
{
    return m_data.nameOverrideStringId;
}

void SaveFileHeader::setNameOverrideStringId(const QString &nameOverrideStringId)
{
    if (m_data.nameOverrideStringId == nameOverrideStringId) {
        return;
    }
    m_data.nameOverrideStringId = nameOverrideStringId;
    Q_EMIT nameOverrideStringIdChanged(m_data.nameOverrideStringId);
}

const QHash<quint32, QString> &SaveFileHeader::unknownValues() const
{
    return m_data.unknownValues;
}

void SaveFileHeader::setUnknownValues(const QHash<quint32, QString> &values)
{
    if (m_data.unknownValues == values) {
        return;
    }

    QHash<quint32, QString> old = m_data.unknownValues;
    m_data.unknownValues.clear();

    for (auto it = values.cbegin(); it != values.cend(); ++it) {
        m_data.unknownValues.insert(it.key(), it.value());

        if (!old.contains(it.key()) || old[it.key()] != it.value()) {
            Q_EMIT unknownValueChanged(it.key(), it.value());
        }
    }

    Q_EMIT unknownValuesChanged(m_data.unknownValues);
}

const QString &SaveFileHeader::unknownValue(quint32 key) const
{
    return m_data.unknownValues[key];
}

void SaveFileHeader::setUnknownValue(quint32 key, const QString &value)
{
    if (m_data.unknownValues.contains(key) && m_data.unknownValues[key] == value) {
        return;
    }

    m_data.unknownValues[key] = value;
    Q_EMIT unknownValueChanged(key, m_data.unknownValues[key]);
}

} // namespace MEASE