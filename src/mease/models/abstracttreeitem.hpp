// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_ABSTRACTTREEITEM_HPP
#define MEASE_ABSTRACTTREEITEM_HPP

#include <QVariant>

namespace MEASE
{
class AbstractTreeItem
{
public:
    explicit AbstractTreeItem(AbstractTreeItem *parent = nullptr);
    virtual ~AbstractTreeItem();

    AbstractTreeItem *parent() const;
    AbstractTreeItem *child(int row);
    int childCount() const;
    int row() const;

    virtual int columnCount() const = 0;
    virtual QVariant data(int column, int role) const = 0;
    virtual bool setData(int column, int role, const QVariant &data) = 0;
    virtual Qt::ItemFlags flags(int column) const;
    virtual bool setFlags(int column, const Qt::ItemFlags &flags);

    virtual bool insertChildren(int position, int count = 1, int columns = 1) = 0;
    virtual bool removeChildren(int position, int count) = 0;

    virtual bool insertColumns(int position, int columns = 1) = 0;
    virtual bool removeColumns(int position, int columns = 1) = 0;

    bool addChild(std::unique_ptr<AbstractTreeItem> &&child);
    bool insertChild(int position, std::unique_ptr<AbstractTreeItem> &&child);
    bool removeChild(int position = -1);
    bool removeChild(AbstractTreeItem *child);
    bool removeChild(const std::unique_ptr<AbstractTreeItem> &child);

    template<typename T, typename... Args>
        requires(std::is_base_of_v<AbstractTreeItem, T>)
    T *addChild(Args &&...args)
    {
        auto uniquePtr = std::make_unique<T>(std::forward<Args>(args)..., this);
        auto *ptr = uniquePtr.get();
        if (addChild(std::move(uniquePtr))) {
            return ptr;
        }
        return nullptr;
    }

    template<typename T, typename... Args>
        requires(std::is_base_of_v<AbstractTreeItem, T>)
    T *insertChild(int position, Args &&...args)
    {
        auto uniquePtr = std::make_unique<T>(std::forward<Args>(args)..., this);
        auto *ptr = uniquePtr.get();
        if (insertChild(position, std::move(uniquePtr))) {
            return ptr;
        }
        return nullptr;
    }

protected:
    std::vector<std::unique_ptr<AbstractTreeItem>> &children();
    const std::vector<std::unique_ptr<AbstractTreeItem>> &children() const;

private:
    AbstractTreeItem *m_parent;

    std::vector<std::unique_ptr<AbstractTreeItem>> m_children;
};
} // namespace MEASE

#endif // MEASE_ABSTRACTTREEITEM_HPP
