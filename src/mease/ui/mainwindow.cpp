#include "mease/ui/mainwindow.hpp"

#include "mease/config.hpp"
#include "mease/ui/pages/landingpage.hpp"
#include "mease/ui/pages/loadingpage.hpp"

#include <KActionCollection>
#include <KHelpMenu>
#include <KLocalizedString>
#include <KStandardAction>

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QUndoStack>

using namespace Qt::StringLiterals;

namespace MEASE
{
class MainWindowPrivate
{
    Q_DISABLE_COPY(MainWindowPrivate)
    Q_DECLARE_PUBLIC(MainWindow)
    MainWindow *q_ptr;

public:
    QUndoStack *undoStack;

    QAction *openAction = nullptr;
    KRecentFilesAction *openRecentAction = nullptr;
    QAction *saveAction = nullptr;
    QAction *saveAsAction = nullptr;
    QAction *reloadAction;
    QAction *quitAction = nullptr;

    QAction *undoAction = nullptr;
    QAction *redoAction = nullptr;
    QAction *revertAction = nullptr;

    explicit MainWindowPrivate(MainWindow *q_ptr)
        : q_ptr{q_ptr}
        , undoStack{new QUndoStack(q_ptr)}
        , reloadAction{new QAction(q_ptr)}
    {
    }

    static void removeAction(KActionCollection *actionCollection, KStandardActions::StandardAction actionKey)
    {
        auto *action = actionCollection->action(KStandardActions::name(actionKey));
        if (action) {
            actionCollection->removeAction(action);
        }
    }

    void saveRecentFile(const QUrl &fileUrl)
    {
        openRecentAction->addUrl(fileUrl);
        openRecentAction->saveEntries(Config::self()->sharedConfig()->group(u"History/RecentFiles"_s));
    }
};

MainWindow::MainWindow(QWidget *parent)
    : KXmlGuiWindow{parent}
    , d_ptr{new MainWindowPrivate(this)}
{
    setMinimumSize(200, 200);

    Q_D(MainWindow);

    auto *actionCollection = this->actionCollection();

    d->openAction = actionCollection->addAction(KStandardAction::Open);
    connect(d->openAction, &QAction::triggered, this, &MainWindow::showOpenFileDialog);

    d->openRecentAction = static_cast<KRecentFilesAction *>(actionCollection->addAction(KStandardAction::OpenRecent));
    connect(d->openRecentAction, &KRecentFilesAction::urlSelected, this, &MainWindow::openSelectedFile);
    d->openRecentAction->setMaxItems(10);
    d->openRecentAction->loadEntries(Config::self()->sharedConfig()->group(u"History/RecentFiles"_s));

    d->saveAction = actionCollection->addAction(KStandardAction::Save);
    d->saveAction->setDisabled(true);

    d->saveAsAction = actionCollection->addAction(KStandardAction::SaveAs);
    d->saveAsAction->setDisabled(true);

    d->reloadAction = actionCollection->addAction(u"reload"_s);
    d->reloadAction->setText(i18n("&Reload"));
    d->reloadAction->setToolTip(i18n("Reload current save file from disk"));
    d->reloadAction->setIcon(QIcon::fromTheme(u"view-refresh"_s));
    d->reloadAction->setDisabled(true);
    KActionCollection::setDefaultShortcut(d->reloadAction, Qt::Key_F5);

    d->quitAction = actionCollection->addAction(KStandardAction::Quit);
    connect(d->quitAction, &QAction::triggered, this, &MainWindow::quit);

    d->undoAction = actionCollection->addAction(KStandardAction::Undo, d->undoStack->createUndoAction(this));
    d->undoAction->setDisabled(true);
    KActionCollection::setDefaultShortcut(d->undoAction, Qt::CTRL | Qt::SHIFT | Qt::Key_Z);
    connect(d->undoStack, &QUndoStack::canUndoChanged, d->undoAction, &QAction::setEnabled);

    d->redoAction = actionCollection->addAction(KStandardAction::Redo, d->undoStack->createRedoAction(this));
    d->redoAction->setDisabled(true);
    KActionCollection::setDefaultShortcut(d->redoAction, Qt::CTRL | Qt::SHIFT | Qt::Key_Y);
    connect(d->undoStack, &QUndoStack::canRedoChanged, d->redoAction, &QAction::setEnabled);

    d->revertAction = actionCollection->addAction(u"revert"_s,
                                                  KStandardAction::revert(
                                                      d->undoStack,
                                                      [d]() {
                                                          while (d->undoStack->canUndo()) {
                                                              d->undoStack->undo();
                                                          }
                                                      },
                                                      this));
    d->revertAction->setDisabled(true);

    connect(d->undoStack, &QUndoStack::canUndoChanged, this, &KXmlGuiWindow::setWindowModified);

    setupGUI({500, 500}, ToolBar | Keys | Save | Create);

    MainWindowPrivate::removeAction(actionCollection, KStandardActions::HelpContents);
    MainWindowPrivate::removeAction(actionCollection, KStandardActions::WhatsThis);

    showLanding();
}

MainWindow::~MainWindow() = default;

void MainWindow::showLanding()
{
    setCentralWidget(new LandingPage(this));
}

void MainWindow::showLoading()
{
    setCentralWidget(new LoadingPage(this));
}

void MainWindow::showEditor()
{
}

void MainWindow::showOpenFileDialog()
{
    auto *fileDialog = new QFileDialog(this, i18nc("@title:window", "Open a save file"));
    fileDialog->setNameFilters({u"%1 (Career*)"_s.arg(i18nc("@label:listbox file dialog filter", "Save files")),
                                u"%1 (*)"_s.arg(i18nc("@label:listbox file dialog filter", "All files"))});
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setOptions(QFileDialog::ReadOnly);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setDirectoryUrl(Config::self()->lastOpenDir());

    connect(fileDialog, &QFileDialog::finished, fileDialog, &QFileDialog::deleteLater);
    connect(fileDialog, &QFileDialog::urlSelected, this, &MainWindow::openSelectedFile);

    fileDialog->show();
}

void MainWindow::quit()
{
    QApplication::quit();
}

void MainWindow::openSelectedFile(const QUrl &fileUrl)
{
    showLoading();

    if (!fileUrl.isLocalFile()) {
        // TODO: Error
        // showLast();
        return;
    }

    const auto localFile = fileUrl.toLocalFile();
    QFile file(localFile);
    if (!file.open(QFile::ReadOnly)) {
        // TODO: Error
        // showLast();
        return;
    }

    Q_D(MainWindow);
    d->saveRecentFile(fileUrl);
    Config::setLastOpenDir(fileUrl);
    Config::self()->save();
}
} // namespace MEASE
