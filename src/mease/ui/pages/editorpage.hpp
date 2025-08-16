// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_EDITORPAGE_HPP
#define MEASE_EDITORPAGE_HPP

#include <QTabWidget>

namespace MEASE
{
class GameSave;
class GameSaveObject;
class EditorPage : public QTabWidget
{
    Q_OBJECT
    Q_PROPERTY(GameSaveObject *gameSave READ gameSave CONSTANT FINAL)

public:
    EditorPage(GameSave &&gameSave, QWidget *parent = nullptr);
    EditorPage(const GameSave &gameSave, QWidget *parent = nullptr);

    GameSaveObject *gameSave() const;

private:
    GameSaveObject *m_gameSave;

    void init();
};
} // namespace MEASE

#endif // MEASE_EDITORPAGE_HPP
