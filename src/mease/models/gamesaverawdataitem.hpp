// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_GAMESAVERAWDATAITEM_HPP
#define MEASE_GAMESAVERAWDATAITEM_HPP

#include "mease/models/abstracttreeitem.hpp"

#include <QMetaProperty>

namespace MEASE
{
class GameSaveRawDataItem : public AbstractTreeItem
{
    GameSaveRawDataItem(const QString &key, QObject *target, const QMetaProperty &property, AbstractTreeItem *parent);

public:
    enum DelegateOption {
        Min,
        Max,
        Decimal
    };

    GameSaveRawDataItem(const QString &key, QObject *target, const char *propertyName, AbstractTreeItem *parent = nullptr);
    ~GameSaveRawDataItem() override;

    std::function<QString(const QVariant &)> displayFormat() const;
    GameSaveRawDataItem *setDisplayFormat(std::function<QString(const QVariant &)> displayFunc);

    const QHash<DelegateOption, QVariant> &delegateOptions() const;
    GameSaveRawDataItem *setDelegateOptions(const QHash<DelegateOption, QVariant> &delegateOptions);

    QVariant delegateOption(DelegateOption option, const QVariant &defaultValue = {}) const;
    GameSaveRawDataItem *setDelegateOption(DelegateOption option, const QVariant &value);

    bool isReadOnly() const;
    GameSaveRawDataItem *setReadOnly(bool value);

    int columnCount() const override;
    QVariant data(int column, int role) const override;
    bool setData(int column, int role, const QVariant &data) override;
    Qt::ItemFlags flags(int column) const override;
    bool insertChildren(int position, int count, int columns) override;
    bool removeChildren(int position, int count) override;
    bool insertColumns(int position, int columns) override;
    bool removeColumns(int position, int columns) override;

private:
    std::function<QString(const QVariant &)> m_displayFunc;
    QHash<DelegateOption, QVariant> m_delegateOptions;
    bool m_readOnly = false;

    QString m_key;
    QObject *m_target;
    QMetaProperty m_property;
};
} // namespace MEASE

#endif // MEASE_GAMESAVERAWDATAITEM_HPP
