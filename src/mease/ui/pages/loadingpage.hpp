#ifndef MEASE_LOADINGPAGE_HPP
#define MEASE_LOADINGPAGE_HPP

#include <QWidget>

namespace MEASE
{
class LoadingPagePrivate;
class LoadingPage : public QWidget
{
    Q_OBJECT
public:
    explicit LoadingPage(QWidget *parent = nullptr);
    ~LoadingPage() override;
};
} // namespace MEASE

#endif // MEASE_LOADINGPAGE_HPP
