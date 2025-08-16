// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "gamesaverawdatamodel.hpp"

#include "mease/data/gamesave.hpp"
#include "mease/models/gamesaverawdataitem.hpp"
#include "mease/models/treeitem.hpp"

#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;

namespace MEASE
{
namespace
{
TreeItem *addSection(AbstractTreeItem *parent, const QString &label)
{
    auto list = std::initializer_list<QVariant>{label, {}, {}};
    return parent->addChild<TreeItem>(std::move(list));
}

GameSaveRawDataItem *addItem(AbstractTreeItem *parent, QObject *target, const QString &propertyName)
{
    return parent->addChild<GameSaveRawDataItem>(propertyName, target, qPrintable(propertyName));
};
};

class GameSaveRawDataModelPrivate
{
    Q_DECLARE_PUBLIC(GameSaveRawDataModel)
    GameSaveRawDataModel *q_ptr;

public:
    GameSaveObject *gameSave = nullptr;

    AbstractTreeItem *rootItem;

    explicit GameSaveRawDataModelPrivate(GameSaveRawDataModel *q_ptr)
        : q_ptr{q_ptr}
        , rootItem{q_ptr->rootItem()}
    {
    }

    void clear()
    {
        Q_Q(GameSaveRawDataModel);
        q->beginResetModel();
        rootItem->removeChildren(0, rootItem->childCount());
        q->endResetModel();
    }

    void setup()
    {
        Q_Q(GameSaveRawDataModel);
        q->beginResetModel();
        rootItem->removeChildren(0, rootItem->childCount());

        {
            auto *headerItem = addSection(rootItem, QStringLiteral("header"));
            addItem(headerItem, gameSave->header(), u"version"_s)->setReadOnly(true);
            addItem(headerItem, gameSave->header(), u"areaNameStringId"_s);
            addItem(headerItem, gameSave->header(), u"areaThumbnailTextureId"_s);
            addItem(headerItem, gameSave->header(), u"gameVersion"_s);
            addItem(headerItem, gameSave->header(), u"requiredDLC"_s);
            addItem(headerItem, gameSave->header(), u"requiredInstallGroup"_s);
            addItem(headerItem, gameSave->header(), u"profileName"_s);
            addItem(headerItem, gameSave->header(), u"profileUniqueName"_s);
            addItem(headerItem, gameSave->header(), u"profileId"_s);
            addItem(headerItem, gameSave->header(), u"levelId"_s);
            addItem(headerItem, gameSave->header(), u"playerLevel"_s);
            addItem(headerItem, gameSave->header(), u"gameCompleted"_s);
            addItem(headerItem, gameSave->header(), u"trialMode"_s);
            addItem(headerItem, gameSave->header(), u"completionPercentage"_s);
            addItem(headerItem, gameSave->header(), u"dateTime"_s);
            addItem(headerItem, gameSave->header(), u"levelTitleId"_s);
            addItem(headerItem, gameSave->header(), u"levelFloorId"_s);
            addItem(headerItem, gameSave->header(), u"levelRegionId"_s);
            addItem(headerItem, gameSave->header(), u"totalPlaytime"_s);
            addItem(headerItem, gameSave->header(), u"nameOverrideStringId"_s);
        }
        q->endResetModel();
    }
};

GameSaveRawDataModel::GameSaveRawDataModel(QObject *parent)
    : AbstractTreeModel{std::make_unique<TreeItem>(
                            QVariantList{i18nc("@title:column", "Key"), i18nc("@title:column", "Read Only"), i18nc("@title:column", "Raw Data")}),
                        parent}
    , d_ptr{new GameSaveRawDataModelPrivate(this)}
{
}

GameSaveRawDataModel::~GameSaveRawDataModel() = default;

GameSaveObject *GameSaveRawDataModel::gameSave() const
{
    Q_D(const GameSaveRawDataModel);
    return d->gameSave;
}

void GameSaveRawDataModel::setGameSave(GameSaveObject *ptr)
{
    Q_D(GameSaveRawDataModel);
    if (ptr == d->gameSave) {
        return;
    }
    d->gameSave = ptr;
    if (d->gameSave) {
        d->setup();
    } else {
        d->clear();
    }
    Q_EMIT gameSaveChanged(d->gameSave);
}
} // namespace MEASE
