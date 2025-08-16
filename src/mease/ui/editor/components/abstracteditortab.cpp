// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "abstracteditortab.hpp"

namespace MEASE
{
AbstractEditorTab::AbstractEditorTab(const QString &title, QWidget *parent)
    : QWidget{parent}
    , m_title{title}
{
}

QString AbstractEditorTab::title() const
{
    return m_title;
}
} // namespace MEASE
