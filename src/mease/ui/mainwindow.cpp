// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "mease/ui/mainwindow.hpp"

#include "mease/data/savefile.hpp"
#include "mease/serialize/datastream.hpp"
#include "mease/services/message.hpp"
#include "mease/ui/dialogs/aboutdialog.hpp"
#include "mease/ui/pages/editorpage.hpp"
#include "mease/ui/pages/landingpage.hpp"
#include "mease/ui/pages/loadingpage.hpp"

#include <QApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QMenuBar>
#include <QMutex>
#include <QSettings>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QThreadPool>
#include <QtSystemDetection>

using namespace Qt::Literals::StringLiterals;

namespace MEASE
{

class MainWindowPrivate
{
    Q_DISABLE_COPY(MainWindowPrivate)
    Q_DECLARE_PUBLIC(MainWindow)
    MainWindow *q_ptr;

public:
    QMutex mutex;

    // Menubar
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    // File menu actions
    QAction *openSaveFileAction;
    QAction *saveSaveFileAction;
    QAction *saveSaveFileAsAction;
    QAction *reloadSaveFileAction;
    QAction *quitAction;

    // Help menu actions
    QAction *openAboutQtDialogAction;
    QAction *openAboutDialogAction;
    AboutDialog *aboutDialog;

    // Contents
    QStackedWidget *centralWidget;
    LandingPage *landingPage;
    LoadingPage *loadingPage;
    EditorPage *editorPage;

    MainWindowPrivate(MainWindow *q_ptr)
        : q_ptr{q_ptr}
        , menuBar{new QMenuBar(q_ptr)}
        , fileMenu{menuBar->addMenu(QString())}
        , editMenu{menuBar->addMenu(QString())}
        , helpMenu{menuBar->addMenu(QString())}
        , openSaveFileAction{new QAction(q_ptr)}
        , saveSaveFileAction{new QAction(q_ptr)}
        , saveSaveFileAsAction{new QAction(q_ptr)}
        , reloadSaveFileAction{new QAction(q_ptr)}
        , quitAction{new QAction(q_ptr)}
        , openAboutQtDialogAction{new QAction(q_ptr)}
        , openAboutDialogAction{new QAction(q_ptr)}
        , aboutDialog{new AboutDialog(q_ptr)}
        , centralWidget{new QStackedWidget(q_ptr)}
        , landingPage{new LandingPage(openSaveFileAction, centralWidget)}
        , loadingPage{new LoadingPage(centralWidget)}
        , editorPage{new EditorPage(centralWidget)}
    {
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
    }

    static QString getSaveFileDir()
    {
        QSettings settings;
        if (settings.contains(u"History/LastOpenDir"_s)) {
            const auto dir = QDir::toNativeSeparators(QDir::cleanPath(settings.value(u"History/LastOpenDir"_s).toString()));
            QFileInfo fileInfo(dir);
            fileInfo.refresh();
            if (fileInfo.isDir()) {
                return dir;
            }
        }

#ifdef Q_OS_WIN
        static const auto docDir = QDir::toNativeSeparators(QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)));
        static const QStringList candidateDir{QDir::toNativeSeparators(QDir::cleanPath(docDir + u"/BioWare/Mass Effect Andromeda/Save"_s)),
                                              QDir::toNativeSeparators(QDir::cleanPath(docDir + u"/BioWare/Mass Effect Andromeda"_s)),
                                              QDir::toNativeSeparators(QDir::cleanPath(docDir + u"/BioWare"_s))};

        for (const auto &dir : candidateDir) {
            QFileInfo fileInfo(dir);
            fileInfo.refresh();
            if (fileInfo.isDir()) {
                return dir;
            }
        }
        return docDir;
#else
        static const auto homeDir = QDir::toNativeSeparators(QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)));
        return homeDir;
#endif
    }

    void setIndex(int index)
    {
        QMutexLocker lock(&mutex); // Probably overkill. But just to be safe.
        centralWidget->setCurrentIndex(index);
    }

    void loadSaveFile(const QString &filePath)
    {
        const int lastIndex = centralWidget->currentIndex();
        setIndex(1);

        QThreadPool::globalInstance()->start([this, filePath, lastIndex]() {
            bool failed = false;

            QFile file(filePath);
            if (file.open(QFile::ReadOnly)) {
                DataStream stream(&file);
                auto saveFileDataRes = stream.read<SaveFileData>();
                if (saveFileDataRes.has_value()) {
                    // TODO
                } else {
                    messageService()->pushError(MainWindow::tr("Failed to open save file"), saveFileDataRes.error());
                    failed = true;
                }
            } else {
                messageService()->pushError(MainWindow::tr("Failed to open save file"), file.errorString());
                failed = true;
            }

            if (failed) {
                setIndex(lastIndex);
            }
        });
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d_ptr{new MainWindowPrivate(this)}
{
    messageService()->setMainWindow(this);

    Q_D(MainWindow);

    setMinimumSize(200, 200);
    resize(500, 500);

    setMenuBar(d->menuBar);
    setCentralWidget(d->centralWidget);

    // Setup actions
    d->openSaveFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen));
    d->saveSaveFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave));
    d->saveSaveFileAsAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSaveAs));
    d->reloadSaveFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ViewRefresh));
    d->quitAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit));

    d->openSaveFileAction->setShortcut({Qt::CTRL | Qt::Key_O});
    d->saveSaveFileAction->setShortcut({Qt::CTRL | Qt::Key_S});
    d->saveSaveFileAsAction->setShortcut({Qt::CTRL | Qt::SHIFT | Qt::Key_S});
    d->reloadSaveFileAction->setShortcut({Qt::CTRL | Qt::Key_R});
    d->quitAction->setShortcut({Qt::CTRL | Qt::Key_Q});

    d->saveSaveFileAction->setDisabled(true);
    d->saveSaveFileAsAction->setDisabled(true);
    d->reloadSaveFileAction->setDisabled(true);

    d->fileMenu->addAction(d->openSaveFileAction);
    d->fileMenu->addAction(d->saveSaveFileAction);
    d->fileMenu->addAction(d->saveSaveFileAsAction);
    d->fileMenu->addAction(d->reloadSaveFileAction);
    d->fileMenu->addSeparator();
    d->fileMenu->addAction(d->quitAction);

    d->helpMenu->addAction(d->openAboutQtDialogAction);
    d->helpMenu->addAction(d->openAboutDialogAction);

    connect(d->openSaveFileAction, &QAction::triggered, this, &MainWindow::openSaveFileActionTriggered);
    connect(d->quitAction, &QAction::triggered, QCoreApplication ::instance(), &QCoreApplication::quit);
    connect(d->openAboutQtDialogAction, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(d->openAboutDialogAction, &QAction::triggered, d->aboutDialog, &QDialog::open);
    connect(d->centralWidget, &QStackedWidget::currentChanged, this, &MainWindow::updateActions);

    d->centralWidget->addWidget(d->landingPage);
    d->centralWidget->addWidget(d->loadingPage);
    d->centralWidget->addWidget(d->editorPage);

    d->retranslateUi();
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

void MainWindow::updateActions()
{
    Q_D(MainWindow);
    const bool notLoading = d->centralWidget->currentIndex() != 1;
    const bool canSave = false && notLoading;

    d->openSaveFileAction->setEnabled(notLoading);
    d->saveSaveFileAction->setEnabled(canSave);
    d->saveSaveFileAsAction->setEnabled(canSave);
    d->reloadSaveFileAction->setEnabled(canSave);
}

void MainWindow::openSaveFileActionTriggered()
{
    static const QStringList nameFilters{u"%1 (Career*)"_s.arg(tr("Save files", "File dialog name filter")),
                                         u"%1 (*)"_s.arg(tr("All files", "File dialog name filter"))};

    auto *fileDialog = new QFileDialog(this, tr("Open a save file", "File dialog title"), MainWindowPrivate::getSaveFileDir());
    fileDialog->setNameFilters(nameFilters);
    fileDialog->setOption(QFileDialog::ReadOnly);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);

    connect(fileDialog, &QFileDialog::finished, fileDialog, &QFileDialog::deleteLater);
    connect(fileDialog, &QFileDialog::fileSelected, this, &MainWindow::openSaveFileSelected);

    fileDialog->open();
}

void MainWindow::openSaveFileSelected(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    QSettings().setValue("History/LastOpenDir", fileInfo.path());

    Q_D(MainWindow);
    d->loadSaveFile(filePath);
}

} // namespace MEASE
