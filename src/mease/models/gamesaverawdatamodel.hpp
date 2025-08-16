// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_GAMESAVERAWDATAMODEL_HPP
#define MEASE_GAMESAVERAWDATAMODEL_HPP

#include "mease/models/abstracttreemodel.hpp"

namespace MEASE
{
class GameSaveObject;
class GameSaveRawDataModelPrivate;
class GameSaveRawDataModel : public AbstractTreeModel
{
    Q_OBJECT
    Q_PROPERTY(MEASE::GameSaveObject *gameSave READ gameSave WRITE setGameSave NOTIFY gameSaveChanged FINAL)
public:
    enum Column {
        KeyColumn = 0,
        ReadOnlyColumn,
        // DescriptionColumn,
        RawDataColumn,
        COLUMN_COUNT
    };
    Q_ENUM(Column)

    explicit GameSaveRawDataModel(QObject *parent = nullptr);
    ~GameSaveRawDataModel() override;

    GameSaveObject *gameSave() const;
    void setGameSave(GameSaveObject *ptr);

Q_SIGNALS:
    void gameSaveChanged(MEASE::GameSaveObject *ptr);

private:
    Q_DECLARE_PRIVATE(GameSaveRawDataModel)
    QScopedPointer<GameSaveRawDataModelPrivate> d_ptr;
};
} // namespace MEASE

#endif // MEASE_GAMESAVERAWDATAMODEL_HPP
