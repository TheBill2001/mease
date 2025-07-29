#ifndef MEASE_MAINWINDOW_HPP
#define MEASE_MAINWINDOW_HPP

#include <KXmlGuiWindow>

namespace MEASE
{
class MainWindowPrivate;
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Q_DECLARE_PRIVATE(MainWindow)
    QScopedPointer<MainWindowPrivate> d_ptr;

private Q_SLOTS:
    void showLanding();
    void showLoading();
    void showEditor();
    void showOpenFileDialog();
    void quit();

    void openSelectedFile(const QUrl &fileUrl);
};
} // namespace MEASE

#endif // MEASE_MAINWINDOW_HPP
