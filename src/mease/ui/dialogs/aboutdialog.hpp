#ifndef MEASE_ABOUTDIALOG_HPP
#define MEASE_ABOUTDIALOG_HPP

#include <QDialog>

namespace MEASE
{

class AboutDialogPrivate;
class AboutDialog final : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog() override;

protected:
    void changeEvent(QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(AboutDialog)
    QScopedPointer<AboutDialogPrivate> d_ptr;
};

} // namespace MEASE

#endif // MEASE_ABOUTDIALOG_HPP
