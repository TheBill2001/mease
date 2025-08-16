// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_GAMESAVERAWDATAVIEW_HPP
#define MEASE_GAMESAVERAWDATAVIEW_HPP

#include <QTreeView>

namespace MEASE
{
class GameSaveRawDataView : public QTreeView
{
    Q_OBJECT
public:
    explicit GameSaveRawDataView(QWidget *parent = nullptr);

    void setModel(QAbstractItemModel *model) override;
};
} // namespace MEASE

#endif // MEASE_GAMESAVERAWDATAVIEW_HPP
