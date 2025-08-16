// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "gamesaverawdataview.hpp"

#include "mease/ui/units.hpp"

#include <QHeaderView>
#include <QStyledItemDelegate>

namespace MEASE
{
GameSaveRawDataView::GameSaveRawDataView(QWidget *parent)
    : QTreeView{parent}
{
    header()->setSectionsMovable(false);
    setAlternatingRowColors(true);
    setItemDelegate(new QStyledItemDelegate(this));
}

void GameSaveRawDataView::setModel(QAbstractItemModel *model)
{
    QTreeView::setModel(model);
    setColumnWidth(0, Units::gridUnit * 10);
    expandAll();
}
} // namespace MEASE
