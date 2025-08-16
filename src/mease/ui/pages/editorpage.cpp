// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "editorpage.hpp"

#include "mease/data/gamesave.hpp"
#include "mease/ui/editor/rawdatatab.hpp"

#include <QTabBar>

namespace MEASE
{
EditorPage::EditorPage(GameSave &&gameSave, QWidget *parent)
    : QTabWidget{parent}
    , m_gameSave{new GameSaveObject(std::move(gameSave), this)}
{
    init();
}

EditorPage::EditorPage(const GameSave &gameSave, QWidget *parent)
    : QTabWidget{parent}
    , m_gameSave{new GameSaveObject(gameSave, this)}
{
    init();
}

GameSaveObject *EditorPage::gameSave() const
{
    return m_gameSave;
}

void EditorPage::init()
{
    m_gameSave->moveToThread(thread());
    m_gameSave->setParent(this);

    setDocumentMode(true);
    tabBar()->setExpanding(true);

    auto *rawDataTab = new RawDataTab(m_gameSave, this);
    addTab(rawDataTab, rawDataTab->title());
}
} // namespace MEASE
