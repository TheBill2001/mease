// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_GAMESAVE_HPP
#define MEASE_GAMESAVE_HPP

#include "mease/data/gamesaveheader.hpp"

#include <QUrl>

namespace MEASE
{
class GameSave
{
    Q_GADGET
    Q_PROPERTY(bool isBigEndian MEMBER isBigEndian)
    Q_PROPERTY(QUrl filePath MEMBER filePath)
    Q_PROPERTY(quint16 version MEMBER version)
    Q_PROPERTY(MEASE::GameSaveHeader header MEMBER header)
public:
    bool isBigEndian = false;
    QUrl filePath;

    quint16 version = 0;
    GameSaveHeader header{};
    QByteArray data{};

    bool operator==(const GameSave &other) const = default;
};

class GameSaveObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isBigEndian READ isBigEndian WRITE setBigEndian NOTIFY isBigEndianChanged FINAL)
    Q_PROPERTY(QUrl filePath READ filePath WRITE setFilePath NOTIFY filePathChanged FINAL)
    Q_PROPERTY(quint16 version READ version WRITE setVersion NOTIFY versionChanged FINAL)
    Q_PROPERTY(MEASE::GameSaveHeaderObject *header READ header CONSTANT FINAL)
public:
    GameSaveObject(const GameSave &data, QObject *parent = nullptr);
    GameSaveObject(GameSave &&data, QObject *parent = nullptr);

    bool isBigEndian() const;
    void setBigEndian(bool value);

    const QUrl &filePath() const;
    void setFilePath(const QUrl &url);

    quint16 version() const;
    void setVersion(quint16 version);

    GameSaveHeaderObject *header() const;

Q_SIGNALS:
    void isBigEndianChanged(bool);
    void filePathChanged(const QUrl &url);
    void versionChanged(quint16 version);

private:
    mutable QMutex m_mutex;
    GameSave m_data;
    GameSaveHeaderObject *m_metadata;
};
} // namespace MEASE

Q_DECLARE_METATYPE(MEASE::GameSave)

#endif // MEASE_GAMESAVE_HPP
