// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_LOADINGPAGE_HPP
#define MEASE_LOADINGPAGE_HPP

#include <QWidget>

namespace MEASE
{
class LoadingPage : public QWidget
{
    Q_OBJECT
public:
    explicit LoadingPage(QWidget *parent = nullptr);
};
} // namespace MEASE

#endif // MEASE_LOADINGPAGE_HPP
