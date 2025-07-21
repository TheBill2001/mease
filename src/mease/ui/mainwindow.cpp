// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "mease/ui/mainwindow.hpp"

#include "mease/core/objectutils.hpp"
#include "mease/ui/aboutdialog.hpp"

#include <QApplication>
#include <QMenuBar>

using namespace Qt::Literals::StringLiterals;

namespace MEASE
{

class MainWindowPrivate
{
    MEASE_DECLARE_QT_PUBLIC(MainWindow)
public:
    // Menubar
    QMenuBar menuBar;
    QMenu fileMenu;
    QMenu editMenu;
    QMenu helpMenu;

    // File menu actions
    QAction openSaveFileAction;
    QAction saveSaveFileAction;
    QAction saveSaveFileAsAction;
    QAction reloadSaveFileAction;
    QAction quitAction;

    // Help menu actions
    QAction openAboutDialogAction;
    AboutDialog aboutDialog;

    MainWindowPrivate(MainWindow *self)
        : q_ptr{self}
        , menuBar{self}
        , fileMenu{MainWindow::tr("&File"), self}
        , editMenu{MainWindow::tr("&Edit"), self}
        , helpMenu{MainWindow::tr("&Help"), self}
        , openSaveFileAction{QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen), AboutDialog::tr("&Open"), self}
        , saveSaveFileAction{QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave), AboutDialog::tr("&Save"), self}
        , saveSaveFileAsAction{QIcon::fromTheme(QIcon::ThemeIcon::DocumentSaveAs), AboutDialog::tr("Save &As..."), self}
        , reloadSaveFileAction{QIcon::fromTheme(QIcon::ThemeIcon::ViewRefresh), AboutDialog::tr("&Reload"), self}
        , quitAction{QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit), AboutDialog::tr("&Quit"), self}
        , openAboutDialogAction{MainWindow::tr("&About..."), self}
        , aboutDialog{self}
    {
        openSaveFileAction.setShortcut({Qt::CTRL | Qt::Key_O});
        saveSaveFileAction.setShortcut({Qt::CTRL | Qt::Key_S});
        saveSaveFileAsAction.setShortcut({Qt::CTRL | Qt::SHIFT | Qt::Key_S});
        reloadSaveFileAction.setShortcut({Qt::CTRL | Qt::Key_R});
        quitAction.setShortcut({Qt::CTRL | Qt::Key_Q});

        menuBar.addMenu(&fileMenu);
        menuBar.addMenu(&editMenu);
        menuBar.addMenu(&helpMenu);

        fileMenu.addAction(&openSaveFileAction);
        fileMenu.addAction(&saveSaveFileAction);
        fileMenu.addAction(&saveSaveFileAsAction);
        fileMenu.addAction(&reloadSaveFileAction);
        fileMenu.addSeparator();
        fileMenu.addAction(&quitAction);

        helpMenu.addAction(MainWindow::tr("About &Qt"), qApp, &QApplication::aboutQt);
        helpMenu.addAction(&openAboutDialogAction);

        connect(&quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

        connect(&openAboutDialogAction, &QAction::triggered, &aboutDialog, &QDialog::open);
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d_ptr{new MainWindowPrivate(this)}
{
    Q_D(MainWindow);

    setMinimumSize(200, 200);
    resize(500, 500);

    setMenuBar(&d->menuBar);
}

MainWindow::~MainWindow() = default;

} // namespace MEASE
