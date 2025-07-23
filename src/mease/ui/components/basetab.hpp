#ifndef MEASE_BASETAB_HPP
#define MEASE_BASETAB_HPP

#include <QScrollArea>

namespace MEASE
{
class BaseTabPrivate;
class BaseTab : public QScrollArea
{
    Q_OBJECT
public:
    explicit BaseTab(QWidget *parent = nullptr);
    ~BaseTab() override;

protected:
    void addWidget(QWidget *widget, int stretch = 0);

private:
    Q_DECLARE_PRIVATE(BaseTab)
    QScopedPointer<BaseTabPrivate> d_ptr;
};
} // namespace MEASE

#endif // MEASE_BASETAB_HPP
