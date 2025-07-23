#ifndef MEASE_LANDINGPAGE_HPP
#define MEASE_LANDINGPAGE_HPP

#include <QWidget>

namespace MEASE
{

class LandingPagePrivate;
class LandingPage final : public QWidget
{
    Q_OBJECT
public:
    LandingPage(QAction *openSaveFileAction, QWidget *widget = nullptr);
    ~LandingPage() override;

protected:
    void changeEvent(QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(LandingPage)
    QScopedPointer<LandingPagePrivate> d_ptr;
};

} // namespace MEASE

#endif // MEASE_LANDINGPAGE_HPP
