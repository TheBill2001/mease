// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_TREEITEM_HPP
#define MEASE_TREEITEM_HPP

#include "mease/models/abstracttreeitem.hpp"

namespace MEASE
{
struct TreeItemData {
    QHash<int, QVariant> data;
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
};

class TreeItem : public AbstractTreeItem
{
public:
    explicit TreeItem(AbstractTreeItem *parent = nullptr);
    TreeItem(const QVariantList &data, AbstractTreeItem *parent = nullptr);
    TreeItem(const QList<TreeItemData> &data, AbstractTreeItem *parent = nullptr);
    ~TreeItem() override;

    int columnCount() const override;
    QVariant data(int column, int role) const override;
    bool setData(int column, int role, const QVariant &data) override;
    Qt::ItemFlags flags(int column) const override;
    bool setFlags(int column, const Qt::ItemFlags &flags) override;

    bool insertChildren(int position, int count, int columns) override;
    bool removeChildren(int position, int count) override;
    bool insertColumns(int position, int columns) override;
    bool removeColumns(int position, int columns) override;

private:
    QList<TreeItemData> m_data;
};
} // namespace MEASE

#endif // MEASE_TREEITEM_HPP
