#ifndef MEASE_LOADINGPAGE_HPP
#define MEASE_LOADINGPAGE_HPP

#include <QWidget>

namespace MEASE
{

class LoadingPagePrivate;
class LoadingPage final : public QWidget
{
    Q_OBJECT
public:
    explicit LoadingPage(QWidget *parent = nullptr);
    ~LoadingPage() override;

protected:
    void changeEvent(QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(LoadingPage)
    QScopedPointer<LoadingPagePrivate> d_ptr;
};

} // namespace MEASE

#endif // MEASE_LOADINGPAGE_HPP
