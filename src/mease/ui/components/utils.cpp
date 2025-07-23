#include "mease/ui/components/utils.hpp"

#include <QLabel>

namespace MEASE
{

void adjustLabelSize(QLabel *label)
{
    label->setMinimumHeight(label->heightForWidth(label->width()));
}

} // namespace MEASE