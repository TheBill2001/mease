// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "loadingpage.hpp"

#include <KBusyIndicatorWidget>
#include <KLocalizedString>

#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

namespace MEASE
{
LoadingPage::LoadingPage(QWidget *parent)
    : QWidget{parent}
{
    auto *vLayout = new QVBoxLayout(this);

    vLayout->addStretch();

    auto *hLayout = new QHBoxLayout();

    hLayout->addStretch();

    auto *busy = new KBusyIndicatorWidget(this);
    busy->start();

    auto *opacity = new QGraphicsOpacityEffect(busy);
    opacity->setOpacity(0.8);
    busy->setGraphicsEffect(opacity);

    hLayout->addWidget(busy, 0, Qt::AlignCenter);

    auto *label = new QLabel(this);
    label->setText(i18n("Loading..."));

    opacity = new QGraphicsOpacityEffect(label);
    opacity->setOpacity(0.8);
    label->setGraphicsEffect(opacity);

    hLayout->addWidget(label, 0, Qt::AlignCenter);

    hLayout->addStretch();

    vLayout->addLayout(hLayout);

    vLayout->addStretch();
}
} // namespace MEASE
