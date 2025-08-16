// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_LANDINGPAGE_HPP
#define MEASE_LANDINGPAGE_HPP

#include <QWidget>

namespace MEASE
{
class LandingPage : public QWidget
{
    Q_OBJECT
public:
    explicit LandingPage(QWidget *parent = nullptr);
};
} // namespace MEASE

#endif // MEASE_LANDINGPAGE_HPP
