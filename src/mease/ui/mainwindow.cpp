#include "mease/ui/mainwindow.hpp"

#include <KToolBar>
#include <QApplication>
#include <QToolBar>

using namespace Qt::StringLiterals;

namespace MEASE
{
MainWindow::MainWindow(QWidget *parent)
    : KMainWindow{parent}
{
    auto *toolbar = toolBar();
    toolbar->addAction(QIcon::fromTheme(u"applications-development"_s), u"&TEST"_s);
    toolbar->setMovable(false);
}
} // namespace MEASE