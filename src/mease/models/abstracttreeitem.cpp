// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "abstracttreeitem.hpp"

namespace MEASE
{
AbstractTreeItem::AbstractTreeItem(AbstractTreeItem *parent)
    : m_parent{parent}
{
}

AbstractTreeItem::~AbstractTreeItem() = default;

AbstractTreeItem *AbstractTreeItem::parent() const
{
    return m_parent;
}

AbstractTreeItem *AbstractTreeItem::child(int row)
{
    return row >= 0 && row < childCount() ? m_children.at(row).get() : nullptr;
}

int AbstractTreeItem::childCount() const
{
    return int(m_children.size());
}

int AbstractTreeItem::row() const
{
    if (!m_parent) {
        return 0;
    }

    const auto it = std::find_if(m_parent->m_children.cbegin(), m_parent->m_children.cend(), [this](const std::unique_ptr<AbstractTreeItem> &treeItem) {
        return treeItem.get() == this;
    });

    if (it != m_parent->m_children.cend()) {
        return std::distance(m_parent->m_children.cbegin(), it);
    }

    Q_ASSERT(false); // should not happen
    return -1;
}

Qt::ItemFlags AbstractTreeItem::flags(int) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool AbstractTreeItem::setFlags(int, const Qt::ItemFlags &)
{
    return false;
}

bool AbstractTreeItem::addChild(std::unique_ptr<AbstractTreeItem> &&child)
{
    if (m_children.size() >= std::numeric_limits<int>::max()) {
        return false;
    }

    m_children.push_back(std::move(child));
    return true;
}

bool AbstractTreeItem::insertChild(int position, std::unique_ptr<AbstractTreeItem> &&child)
{
    if (position < 0 || position > qsizetype(m_children.size())) {
        return false;
    }

    m_children.insert(m_children.begin() + position, std::move(child));
    return true;
}

bool AbstractTreeItem::removeChild(int position)
{
    if (position < 0) {
        m_children.erase(m_children.end() - 1);
        return true;
    }

    if (position >= qsizetype(m_children.size())) {
        return false;
    }

    m_children.erase(m_children.begin() + position);
    return true;
}

bool AbstractTreeItem::removeChild(AbstractTreeItem *child)
{
    if (!child) {
        return false;
    }

    const auto it = std::find_if(m_children.begin(), m_children.end(), [child](const std::unique_ptr<AbstractTreeItem> &item) {
        return item.get() == child;
    });

    if (it != m_children.cend()) {
        m_children.erase(it);
        return true;
    }

    return false;
}

bool AbstractTreeItem::removeChild(const std::unique_ptr<AbstractTreeItem> &child)
{
    return removeChild(child.get());
}

std::vector<std::unique_ptr<AbstractTreeItem>> &AbstractTreeItem::children()
{
    return m_children;
}

const std::vector<std::unique_ptr<AbstractTreeItem>> &AbstractTreeItem::children() const
{
    return m_children;
}
} // namespace MEASE
