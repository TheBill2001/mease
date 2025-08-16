// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "gamesaverawdataitem.hpp"

#include "mease/models/gamesaverawdatamodel.hpp"

namespace MEASE
{
namespace
{
QString defaultFormatFunc(const QVariant &value)
{
    if (!value.isValid() || value.isNull()) {
        return {};
    }

    switch (value.typeId()) {
    case QMetaType::Bool:
        return value.toBool() ? GameSaveRawDataModel::tr("True") : GameSaveRawDataModel::tr("False");
    }

    return value.toString();
}
}

GameSaveRawDataItem::GameSaveRawDataItem(const QString &key, QObject *target, const QMetaProperty &property, AbstractTreeItem *parent)
    : AbstractTreeItem{parent}
    , m_displayFunc{defaultFormatFunc}
    , m_key{key}
    , m_target{target}
    , m_property{property}
{
    Q_ASSERT(property.isValid());
}

GameSaveRawDataItem::GameSaveRawDataItem(const QString &key, QObject *target, const char *propertyName, AbstractTreeItem *parent)
    : GameSaveRawDataItem{key, target, target->metaObject()->property(target->metaObject()->indexOfProperty(propertyName)), parent}
{
}

GameSaveRawDataItem::~GameSaveRawDataItem() = default;

std::function<QString(const QVariant &)> GameSaveRawDataItem::displayFormat() const
{
    return m_displayFunc;
}

GameSaveRawDataItem *GameSaveRawDataItem::setDisplayFormat(std::function<QString(const QVariant &)> displayFunc)
{
    m_displayFunc = displayFunc;
    return this;
}

const QHash<GameSaveRawDataItem::DelegateOption, QVariant> &GameSaveRawDataItem::delegateOptions() const
{
    return m_delegateOptions;
}

GameSaveRawDataItem *GameSaveRawDataItem::setDelegateOptions(const QHash<DelegateOption, QVariant> &delegateOptions)
{
    m_delegateOptions = delegateOptions;
    return this;
}

QVariant GameSaveRawDataItem::delegateOption(DelegateOption option, const QVariant &defaultValue) const
{
    return m_delegateOptions.value(option, defaultValue);
}

GameSaveRawDataItem *GameSaveRawDataItem::setDelegateOption(DelegateOption option, const QVariant &value)
{
    m_delegateOptions[option] = value;
    return this;
}

bool GameSaveRawDataItem::isReadOnly() const
{
    return m_readOnly;
}

GameSaveRawDataItem *GameSaveRawDataItem::setReadOnly(bool value)
{
    m_readOnly = value;
    return this;
}

int GameSaveRawDataItem::columnCount() const
{
    return GameSaveRawDataModel::COLUMN_COUNT;
}

QVariant GameSaveRawDataItem::data(int column, int role) const
{
    switch (column) {
    case GameSaveRawDataModel::KeyColumn:
        if (role == Qt::DisplayRole) {
            return m_key;
        }
        break;
    case GameSaveRawDataModel::ReadOnlyColumn:
        if (role == Qt::DisplayRole && m_readOnly) {
            return QObject::tr("Yes");
        }
        break;
    case GameSaveRawDataModel::RawDataColumn:
        if (role == Qt::DisplayRole) {
            auto value = m_property.read(m_target);
            if (m_displayFunc) {
                return m_displayFunc(value);
            }
            return value;
        } else if (role == Qt::EditRole) {
            return m_property.read(m_target);
        }
    }
    return {};
}

bool GameSaveRawDataItem::setData(int column, int role, const QVariant &data)
{
    if (column == GameSaveRawDataModel::RawDataColumn && role == Qt::EditRole) {
        m_property.write(m_target, data);
        return true;
    }
    return false;
}

Qt::ItemFlags GameSaveRawDataItem::flags(int column) const
{
    auto flags = AbstractTreeItem::flags(column);
    if (column == GameSaveRawDataModel::RawDataColumn && !m_readOnly) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

bool GameSaveRawDataItem::insertChildren(int position, int count, int columns)
{
    Q_UNUSED(position)
    Q_UNUSED(count)
    Q_UNUSED(columns)
    return false;
}

bool GameSaveRawDataItem::removeChildren(int position, int count)
{
    Q_UNUSED(position)
    Q_UNUSED(count)
    return false;
}

bool GameSaveRawDataItem::insertColumns(int position, int columns)
{
    Q_UNUSED(position)
    Q_UNUSED(columns)
    return false;
}

bool GameSaveRawDataItem::removeColumns(int position, int columns)
{
    Q_UNUSED(position)
    Q_UNUSED(columns)
    return false;
}
} // namespace MEASE
