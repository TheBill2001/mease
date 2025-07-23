#include "mease/ui/pages/loadingpage.hpp"

#include "mease/ui/components/utils.hpp"

#include <QEvent>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>

namespace MEASE
{

class LoadingPagePrivate final
{
    Q_DISABLE_COPY(LoadingPagePrivate)
    Q_DECLARE_PUBLIC(LoadingPage)
    LoadingPage *q_ptr;

public:
    QLabel *label;
    QProgressBar *progressBar;
    QVBoxLayout *layout;

    explicit LoadingPagePrivate(LoadingPage *q_ptr)
        : q_ptr{q_ptr}
        , label{new QLabel(q_ptr)}
        , progressBar{new QProgressBar(q_ptr)}
        , layout(new QVBoxLayout(q_ptr))
    {
    }

    void retranslateUi()
    {
        label->setText(LoadingPage::tr("Loading..."));
        adjustLabelSize(label);
    }
};

LoadingPage::LoadingPage(QWidget *parent)
    : QWidget{parent}
    , d_ptr{new LoadingPagePrivate(this)}
{
    Q_D(LoadingPage);

    d->label->setMinimumWidth(150);
    d->label->setMaximumWidth(300);
    d->label->setWordWrap(true);
    d->label->setAlignment(Qt::AlignCenter);

    d->progressBar->setMinimum(0);
    d->progressBar->setMaximum(0);
    d->progressBar->setMinimumWidth(150);
    d->progressBar->setMaximumWidth(300);

    d->layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Expanding));
    d->layout->addWidget(d->label, 0, Qt::AlignCenter);
    d->layout->addWidget(d->progressBar, 0, Qt::AlignCenter);
    d->layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Expanding));

    d->retranslateUi();
}

LoadingPage::~LoadingPage() = default;

void LoadingPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        Q_D(LoadingPage);
        d->retranslateUi();
    }
    QWidget::changeEvent(event);
}

} // namespace MEASE