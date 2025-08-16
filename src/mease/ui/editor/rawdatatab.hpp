// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_RAWDATATAB_HPP
#define MEASE_RAWDATATAB_HPP

#include "mease/ui/editor/components/abstracteditortab.hpp"

namespace MEASE
{
class GameSaveObject;
class RawDataTab : public AbstractEditorTab
{
    Q_OBJECT
public:
    explicit RawDataTab(GameSaveObject *gameSave, QWidget *parent = nullptr);
};
} // namespace MEASE

#endif // MEASE_RAWDATATAB_HPP
