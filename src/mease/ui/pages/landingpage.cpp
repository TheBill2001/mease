#include "mease/ui/pages/landingpage.hpp"

#include "mease/ui/components/actionbutton.hpp"
#include "mease/ui/components/utils.hpp"

#include <QAction>
#include <QEvent>
#include <QLabel>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;

namespace MEASE
{

class LandingPagePrivate final
{
    Q_DISABLE_COPY(LandingPagePrivate)
    Q_DECLARE_PUBLIC(LandingPage)
    LandingPage *q_ptr;

public:
    QAction *action;
    QLabel *label;
    ActionPushButton *button;
    QVBoxLayout *layout;

    explicit LandingPagePrivate(QAction *openSaveFileAction, LandingPage *q_ptr)
        : q_ptr{q_ptr}
        , action{openSaveFileAction}
        , label{new QLabel(q_ptr)}
        , button(new ActionPushButton(action, q_ptr))
        , layout(new QVBoxLayout(q_ptr))
    {
    }

    void retranslateUi()
    {
        label->setText(LandingPage::tr("Open a save file to continue"));
        adjustLabelSize(label);
    }
};

LandingPage::LandingPage(QAction *openSaveFileAction, QWidget *widget)
    : QWidget{widget}
    , d_ptr{new LandingPagePrivate(openSaveFileAction, this)}
{
    Q_D(LandingPage);

    d->label->setMinimumWidth(150);
    d->label->setMaximumWidth(300);
    d->label->setWordWrap(true);
    d->label->setAlignment(Qt::AlignCenter);

    d->layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Expanding));
    d->layout->addWidget(d->label, 0, Qt::AlignCenter);
    d->layout->addWidget(d->button, 0, Qt::AlignCenter);
    d->layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Expanding));

    d->retranslateUi();
}

LandingPage::~LandingPage() = default;

void LandingPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        Q_D(LandingPage);
        d->retranslateUi();
    }
    QWidget::changeEvent(event);
}

} // namespace MEASE