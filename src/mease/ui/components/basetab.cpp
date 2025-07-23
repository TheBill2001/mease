#include "mease/ui/components/basetab.hpp"

#include <QScrollArea>
#include <QVBoxLayout>

#include <QLabel>

namespace MEASE
{
class BaseTabPrivate
{
    Q_DISABLE_COPY(BaseTabPrivate)
    Q_DECLARE_PUBLIC(BaseTab)
    BaseTab *q_ptr;

public:
    QWidget *centralWidget;
    QVBoxLayout *layout;

    explicit BaseTabPrivate(BaseTab *q_ptr)
        : q_ptr{q_ptr}
        , centralWidget{new QWidget(q_ptr)}
        , layout{new QVBoxLayout(centralWidget)}
    {
    }
};

BaseTab::BaseTab(QWidget *parent)
    : QScrollArea{parent}
    , d_ptr{new BaseTabPrivate(this)}
{
    Q_D(BaseTab);
    setWidget(d->centralWidget);
    setWidgetResizable(true);

    d->layout->addStretch(0);

    for (auto i = 0; i < 10; ++i) {
        auto label =
            new QLabel(QStringLiteral("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas tempus ultrices nulla, et lobortis quam vulputate ac. "
                                      "In efficitur tincidunt elit ac lobortis. Orci varius natoque penatibus et magnis dis parturient montes, nascetur "
                                      "ridiculus mus. Aenean tempor fermentum ligula et laoreet. Phasellus ac eros viverra, fermentum libero ac, suscipit est. "
                                      "Aliquam et leo ligula. Donec molestie imperdiet ligula, quis euismod nisl tincidunt et. "));
        label->setWordWrap(true);
        addWidget(label);
    }
}

BaseTab::~BaseTab() = default;

void BaseTab::addWidget(QWidget *widget, int stretch)
{
    if (!widget) {
        return;
    }

    Q_D(BaseTab);
    d->layout->insertWidget(d->layout->count() - 1, widget, stretch);
}
} // namespace MEASE