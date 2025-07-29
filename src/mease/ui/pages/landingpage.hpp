#ifndef MEASE_LANDINGPAGE_HPP
#define MEASE_LANDINGPAGE_HPP

#include <QWidget>

namespace MEASE
{
class LandingPagePrivate;
class LandingPage : public QWidget
{
    Q_OBJECT
public:
    explicit LandingPage(QWidget *parent = nullptr);
    ~LandingPage() override;
};
} // namespace MEASE

#endif // MEASE_LANDINGPAGE_HPP
