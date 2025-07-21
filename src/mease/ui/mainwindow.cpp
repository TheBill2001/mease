// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "mease/ui/mainwindow.hpp"

#include "mease/core/objectutils.hpp"
#include "mease/ui/aboutdialog.hpp"
#include "mease/ui/components/actionbutton.hpp"

#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QMenuBar>
#include <QSizePolicy>
#include <QSpacerItem>

using namespace Qt::Literals::StringLiterals;

namespace MEASE
{

class MainWindowPrivate
{
    MEASE_DEFINE_QT_PRIVATE(MainWindow)
public:
    // Menubar
    QMenuBar *menuBar = nullptr;
    QMenu *fileMenu = nullptr;
    QMenu *editMenu = nullptr;
    QMenu *helpMenu = nullptr;

    // File menu actions
    QAction *openSaveFileAction = nullptr;
    QAction *saveSaveFileAction = nullptr;
    QAction *saveSaveFileAsAction = nullptr;
    QAction *reloadSaveFileAction = nullptr;
    QAction *quitAction = nullptr;

    // Help menu actions
    QAction *openAboutQtDialogAction = nullptr;
    QAction *openAboutDialogAction = nullptr;
    AboutDialog *aboutDialog = nullptr;

    // Landing page
    QWidget *landingPage = nullptr;
    QLabel *landingLabel = nullptr;

    // Editor page
    QWidget *editorPage = nullptr;

    MainWindowPrivate(MainWindow *self)
        : q_ptr{self}
        , menuBar{new QMenuBar(self)}
        , aboutDialog{new AboutDialog(self)}
    {
        // Setup action
        {
            fileMenu = menuBar->addMenu(QString());
            editMenu = menuBar->addMenu(QString());
            helpMenu = menuBar->addMenu(QString());
            self->setMenuBar(menuBar);

            openSaveFileAction = fileMenu->addAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen), {});
            saveSaveFileAction = fileMenu->addAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave), {});
            saveSaveFileAsAction = fileMenu->addAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSaveAs), {});
            reloadSaveFileAction = fileMenu->addAction(QIcon::fromTheme(QIcon::ThemeIcon::ViewRefresh), {});
            fileMenu->addSeparator();
            quitAction = fileMenu->addAction(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit), {});

            openAboutQtDialogAction = helpMenu->addAction({}, qApp, &QApplication::aboutQt);
            openAboutDialogAction = helpMenu->addAction(QString());

            openSaveFileAction->setShortcut({Qt::CTRL | Qt::Key_O});
            saveSaveFileAction->setShortcut({Qt::CTRL | Qt::Key_S});
            saveSaveFileAsAction->setShortcut({Qt::CTRL | Qt::SHIFT | Qt::Key_S});
            reloadSaveFileAction->setShortcut({Qt::CTRL | Qt::Key_R});
            quitAction->setShortcut({Qt::CTRL | Qt::Key_Q});

            saveSaveFileAction->setDisabled(true);
            saveSaveFileAsAction->setDisabled(true);
            reloadSaveFileAction->setDisabled(true);

            connect(quitAction, &QAction::triggered, QCoreApplication ::instance(), &QCoreApplication::quit);
            connect(openAboutDialogAction, &QAction::triggered, aboutDialog, &QDialog::open);
        }

        makeLandingPage();

        retranslateUi();
    }

    void makeLandingPage()
    {
        if (landingPage) {
            return;
        }

        Q_Q(MainWindow);
        landingPage = new QWidget();
        auto *layout = new QGridLayout(landingPage);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), 0, 0, 1, 3);
        layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum), 1, 0, 2, 1);
        layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum), 1, 2, 2, 1);
        layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), 3, 0, 1, 3);

        landingLabel = new QLabel(landingPage);
        landingLabel->setMaximumWidth(250);
        landingLabel->setWordWrap(true);
        landingLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(landingLabel, 1, 1, Qt::AlignCenter);

        auto *button = new ActionPushButton(openSaveFileAction, landingPage);
        layout->addWidget(button, 2, 1, Qt::AlignCenter);

        q->setCentralWidget(landingPage);
        clearEditor();
    }

    void clearLanding()
    {
        landingPage = nullptr;
        landingLabel = nullptr;
    }

    void makeEditor()
    {
        if (editorPage) {
            return;
        }

        Q_Q(MainWindow);
        q->setCentralWidget(editorPage);
        clearLanding();
    }

    void clearEditor()
    {
        editorPage = nullptr;
    }

    void retranslateUi()
    {
        fileMenu->setTitle(MainWindow::tr("&File"));
        editMenu->setTitle(MainWindow::tr("&Edit"));
        helpMenu->setTitle(MainWindow::tr("&Help"));

        openSaveFileAction->setText(AboutDialog::tr("&Open"));
        saveSaveFileAction->setText(AboutDialog::tr("&Save"));
        saveSaveFileAsAction->setText(AboutDialog::tr("Save &As..."));
        reloadSaveFileAction->setText(AboutDialog::tr("&Reload"));
        quitAction->setText(AboutDialog::tr("&Quit"));

        openAboutQtDialogAction->setText(MainWindow::tr("About &Qt"));
        openAboutDialogAction->setText(MainWindow::tr("&About..."));

        if (landingPage && landingLabel) {
            landingLabel->setText(MainWindow::tr("Open a save file to continue"));
        }
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d_ptr{new MainWindowPrivate(this)}
{
    setMinimumSize(200, 200);
    resize(500, 500);
}

MainWindow::~MainWindow() = default;

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        Q_D(MainWindow);
        d->retranslateUi();
    }

    QWidget::changeEvent(event);
}

} // namespace MEASE
