// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "abstracttreemodel.hpp"

#include "mease/models/abstracttreeitem.hpp"

namespace MEASE
{
AbstractTreeModel::AbstractTreeModel(std::unique_ptr<AbstractTreeItem> &&rootItem, QObject *parent)
    : QAbstractItemModel{parent}
    , m_rootItem{std::move(rootItem)}
{
}

AbstractTreeModel::~AbstractTreeModel() = default;

QVariant AbstractTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return orientation == Qt::Horizontal ? m_rootItem->data(section, role) : QVariant{};
}

Qt::ItemFlags AbstractTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    auto *item = getItem(index);
    return item ? item->flags(index.column()) : Qt::NoItemFlags;
}

int AbstractTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() > 0) {
        return 0;
    }

    const auto *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

int AbstractTreeModel::columnCount(const QModelIndex &) const
{
    return m_rootItem->columnCount();
}

QModelIndex AbstractTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0) {
        return {};
    }

    auto *parentItem = getItem(parent);
    if (!parentItem) {
        return {};
    }

    if (auto *childItem = parentItem->child(row)) {
        return createIndex(row, column, childItem);
    }

    return {};
}

QModelIndex AbstractTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return {};
    }

    auto *childItem = getItem(index);
    auto *parentItem = childItem ? childItem->parent() : nullptr;

    return (parentItem != m_rootItem.get() && parentItem != nullptr) ? createIndex(parentItem->row(), 0, parentItem) : QModelIndex{};
}

QVariant AbstractTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }

    auto *item = getItem(index);

    return item ? item->data(index.column(), role) : QVariant{};
}

bool AbstractTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    auto *item = getItem(index);
    if (!item) {
        return false;
    }

    bool result = item->setData(index.column(), role, value);

    if (result) {
        Q_EMIT dataChanged(index, index, {Qt::DisplayRole, role});
    }

    return result;
}

AbstractTreeItem *AbstractTreeModel::rootItem() const
{
    return m_rootItem.get();
}

bool AbstractTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    bool result = m_rootItem->setData(section, role, value);

    if (result) {
        Q_EMIT headerDataChanged(orientation, section, section);
    }

    return result;
}

bool AbstractTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    beginInsertColumns(parent, position, position + columns - 1);
    const bool success = m_rootItem->insertColumns(position, columns);
    endInsertColumns();
    return success;
}

bool AbstractTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    beginRemoveColumns(parent, position, position + columns - 1);
    const bool success = m_rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (m_rootItem->columnCount() == 0) {
        removeRows(0, rowCount());
    }

    return success;
}

bool AbstractTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    auto *parentItem = getItem(parent);
    if (!parentItem) {
        return false;
    }

    beginInsertRows(parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position, rows, m_rootItem->columnCount());
    endInsertRows();

    return success;
}

bool AbstractTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    auto *parentItem = getItem(parent);
    if (!parentItem) {
        return false;
    }

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

AbstractTreeItem *AbstractTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (auto *item = static_cast<AbstractTreeItem *>(index.internalPointer()))
            return item;
    }
    return m_rootItem.get();
}
} // namespace MEASE
