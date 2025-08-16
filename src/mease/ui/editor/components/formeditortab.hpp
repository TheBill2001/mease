// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_FORMEDITORTAB_HPP
#define MEASE_FORMEDITORTAB_HPP

#include "abstracteditortab.hpp"

namespace MEASE
{
class FormEditorTab : public AbstractEditorTab
{
    Q_OBJECT
protected:
    FormEditorTab(const QString &title, QWidget *parent = nullptr);

public:
    // TODO
};
} // namespace MEASE

#endif // MEASE_FORMEDITORTAB_HPP
