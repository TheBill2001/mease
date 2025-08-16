// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "treeitem.hpp"

namespace MEASE
{
TreeItem::TreeItem(AbstractTreeItem *parent)
    : AbstractTreeItem{parent}
{
}

TreeItem::TreeItem(const QVariantList &data, AbstractTreeItem *parent)
    : AbstractTreeItem{parent}
{
    for (const auto &datum : data) {
        m_data.emplace_back(QHash<int, QVariant>{{Qt::DisplayRole, datum}}, Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }
}

TreeItem::TreeItem(const QList<TreeItemData> &data, AbstractTreeItem *parent)
    : AbstractTreeItem{parent}
    , m_data{data}
{
}

TreeItem::~TreeItem() = default;

int TreeItem::columnCount() const
{
    return m_data.count();
}

QVariant TreeItem::data(int column, int role) const
{
    if (column < 0 || column >= m_data.count()) {
        return {};
    }

    return m_data[column].data.value(role, {});
}

bool TreeItem::setData(int column, int role, const QVariant &data)
{
    if (column < 0 || column >= m_data.count()) {
        return false;
    }

    if (!m_data[column].flags.testFlag(Qt::ItemIsEditable)) {
        return false;
    }

    m_data[column].data[role] = data;
    return true;
}

Qt::ItemFlags TreeItem::flags(int column) const
{
    if (column < 0 || column >= m_data.count()) {
        return Qt::NoItemFlags;
    }

    return m_data[column].flags;
}

bool TreeItem::setFlags(int column, const Qt::ItemFlags &flags)
{
    if (column < 0 || column >= m_data.count()) {
        return false;
    }

    m_data[column].flags = flags;
    return true;
}

bool TreeItem::insertChildren(int position, int count, int columns)
{
    if ((position < 0) || (position > qsizetype(children().size())) || (children().size() + count >= std::numeric_limits<int>::max())) {
        return false;
    }

    for (int row = 0; row < count; ++row) {
        QList<TreeItemData> data(columns);
        children().insert(children().cbegin() + position, std::make_unique<TreeItem>(data, this));
    }

    return true;
}

bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > qsizetype(children().size())) {
        return false;
    }

    for (int row = 0; row < count; ++row) {
        children().erase(children().cbegin() + position);
    }

    return true;
}

bool TreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > m_data.size()) {
        return false;
    }

    for (int column = 0; column < columns; ++column) {
        m_data.insert(position, {QHash<int, QVariant>{}, Qt::ItemIsSelectable | Qt::ItemIsEnabled});
    }

    for (auto &child : std::as_const(children())) {
        child->insertColumns(position, columns);
    }

    return true;
}

bool TreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > m_data.size()) {
        return false;
    }

    for (int column = 0; column < columns; ++column) {
        m_data.remove(position);
    }

    for (auto &child : std::as_const(children())) {
        child->removeColumns(position, columns);
    }

    return true;
}
} // namespace MEASE
