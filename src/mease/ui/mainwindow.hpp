#ifndef MEASE_MAINWINDOW_HPP
#define MEASE_MAINWINDOW_HPP

#include <KMainWindow>

namespace MEASE
{
class MainWindow : public KMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
};
} // namespace MEASE

#endif // MEASE_MAINWINDOW_HPP
