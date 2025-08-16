// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_ABSTRACTTREEMODEL_HPP
#define MEASE_ABSTRACTTREEMODEL_HPP

#include <QAbstractItemModel>

namespace MEASE
{
class AbstractTreeItem;
class AbstractTreeModel : public QAbstractItemModel
{
    Q_OBJECT
protected:
    AbstractTreeModel(std::unique_ptr<AbstractTreeItem> &&rootItem, QObject *parent = nullptr);

public:
    ~AbstractTreeModel() override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns, const QModelIndex &parent = {}) override;
    bool removeColumns(int position, int columns, const QModelIndex &parent = {}) override;
    bool insertRows(int position, int rows, const QModelIndex &parent = {}) override;
    bool removeRows(int position, int rows, const QModelIndex &parent = {}) override;

protected:
    AbstractTreeItem *rootItem() const;

private:
    AbstractTreeItem *getItem(const QModelIndex &index) const;

    std::unique_ptr<AbstractTreeItem> m_rootItem;
};
} // namespace MEASE

#endif // MEASE_ABSTRACTTREEMODEL_HPP
