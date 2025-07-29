#include "landingpage.hpp"

#include <KLocalizedString>

#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QVBoxLayout>

using namespace Qt::StringLiterals;

namespace MEASE
{
LandingPage::LandingPage(QWidget *parent)
    : QWidget{parent}
{
    auto *layout = new QVBoxLayout(this);
    layout->setSpacing(16);

    layout->addStretch();

    {
        auto *label = new QLabel(this);
        label->setMaximumWidth(400);
        label->setText(QStringLiteral(R"(¯\_(ツ)_/¯)"));
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

        auto *opacity = new QGraphicsOpacityEffect(label);
        opacity->setOpacity(0.8);
        label->setGraphicsEffect(opacity);

        layout->addWidget(label, 0, Qt::AlignCenter);
    }

    {
        auto *label = new QLabel(this);
        label->setWordWrap(true);
        label->setMaximumWidth(400);
        label->setText(i18n("Open a save file to continue..."));
        label->setAlignment(Qt::AlignCenter);
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

        auto *opacity = new QGraphicsOpacityEffect(label);
        opacity->setOpacity(0.8);
        label->setGraphicsEffect(opacity);

        layout->addWidget(label, 0, Qt::AlignCenter);
    }

    layout->addStretch();
}

LandingPage::~LandingPage() = default;
} // namespace MEASE
