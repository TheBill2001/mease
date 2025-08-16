// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "rawdatatab.hpp"

#include "mease/data/gamesave.hpp"
#include "mease/models/gamesaverawdatamodel.hpp"
#include "mease/ui/editor/components/gamesaverawdataview.hpp"

#include <QVBoxLayout>

#include <KLocalizedString>

namespace MEASE
{
RawDataTab::RawDataTab(GameSaveObject *gameSave, QWidget *parent)
    : AbstractEditorTab{i18n("Raw Data"), parent}
{
#ifdef QT_DEBUG
    Q_ASSERT(gameSave != nullptr);
#else
    // TODO: error
    return;
#endif

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    auto *model = new GameSaveRawDataModel(this);
    model->setGameSave(gameSave);

    auto *view = new GameSaveRawDataView(this);
    view->setModel(model);

    layout->addWidget(view);
}
} // namespace MEASE
