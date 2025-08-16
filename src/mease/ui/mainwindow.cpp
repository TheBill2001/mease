// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "mease/ui/mainwindow.hpp"

#include "data/gamesave.hpp"
#include "mease/config.hpp"
#include "mease/serialize/serializer.hpp"
#include "mease/ui/pages/editorpage.hpp"
#include "mease/ui/pages/landingpage.hpp"
#include "mease/ui/pages/loadingpage.hpp"
#include "mease/ui/units.hpp"
#include "mease/utils/services/messageservice.hpp"

#include <QCoreApplication>
#include <QFile>
#include <QFileDialog>
#include <QThreadPool>

#include <KActionCollection>
#include <KLocalizedString>
#include <KStandardAction>

using namespace Qt::StringLiterals;

namespace MEASE
{
MainWindow::MainWindow(QWidget *parent)
    : KXmlGuiWindow{parent}
{
    setMinimumSize(Units::gridUnit * 34, Units::gridUnit * 34);

    auto *actionCollection = this->actionCollection();

    {
        auto *openAction = actionCollection->addAction(KStandardAction::Open);
        connect(openAction, &QAction::triggered, this, [this]() {
            auto *fileDialog = new QFileDialog(this, i18nc("@title:window", "Open a save file"));
            fileDialog->setAttribute(Qt::WA_DeleteOnClose, true);
            fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
            fileDialog->setOptions(QFileDialog::ReadOnly);
            fileDialog->setFileMode(QFileDialog::ExistingFile);
            fileDialog->setMimeTypeFilters({u"application/mass-effect-andromeda-save"_s, u"application/octet-stream"_s});
            fileDialog->setDirectoryUrl(Config::self()->lastOpenDir());
            fileDialog->setAttribute(Qt::WA_DeleteOnClose, true);

            connect(fileDialog, &QFileDialog::urlSelected, this, &MainWindow::openSelectedFile);

            fileDialog->open();
        });
    }

    {
        auto *openRecentAction = static_cast<KRecentFilesAction *>(actionCollection->addAction(KStandardAction::OpenRecent));
        connect(openRecentAction, &KRecentFilesAction::urlSelected, this, &MainWindow::openSelectedFile);
        openRecentAction->setMaxItems(10);
        openRecentAction->loadEntries(Config::self()->sharedConfig()->group(u"History/RecentFiles"_s));
    }

    {
        auto *saveAction = actionCollection->addAction(KStandardAction::Save);
        saveAction->setDisabled(true);
    }

    {
        auto *saveAsAction = actionCollection->addAction(KStandardAction::SaveAs);
        saveAsAction->setDisabled(true);
    }

    {
        auto *reloadAction = actionCollection->addAction(u"reload"_s);
        reloadAction->setText(i18n("&Reload"));
        reloadAction->setToolTip(i18n("Reload current save file from disk"));
        reloadAction->setIcon(QIcon::fromTheme(u"view-refresh"_s));
        reloadAction->setDisabled(true);
        KActionCollection::setDefaultShortcut(reloadAction, Qt::CTRL | Qt::Key_F5);
    }

    {
        auto *quitAction = actionCollection->addAction(KStandardAction::Quit);
        connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    }

    setupGUI(minimumSize(), ToolBar | Keys | Save | Create);

    actionCollection->removeAction(actionCollection->action(KStandardActions::name(KStandardActions::HelpContents)));
    actionCollection->removeAction(actionCollection->action(KStandardActions::name(KStandardActions::WhatsThis)));

    messageService()->setMainWindow(this);

    showLanding();
}

void MainWindow::saveRecentFile(const QUrl &fileUrl)
{
    auto *action = static_cast<KRecentFilesAction *>(actionCollection()->action(KStandardActions::name(KStandardActions::OpenRecent)));
    action->addUrl(fileUrl);
    action->saveEntries(Config::self()->sharedConfig()->group(u"History/RecentFiles"_s));
}

void MainWindow::showLanding()
{
    setWindowTitle({});
    setCentralWidget(new LandingPage());
}

void MainWindow::showLoading()
{
    setWindowTitle({});
    setCentralWidget(new LoadingPage());
}

void MainWindow::showEditor(const GameSave &gameSave)
{
    setWindowTitle(gameSave.filePath.fileName());
    setCentralWidget(new EditorPage(std::move(gameSave)));
}

void MainWindow::openSelectedFile(const QUrl &fileUrl)
{
    showLoading();

    if (!fileUrl.isLocalFile()) {
        messageService()->pushError(i18n("Failed to open save file"), i18n("Unsupported file path: %1", fileUrl.toString()));
        showLanding();
        return;
    }

    QThreadPool::globalInstance()->start([mainWindow = this, fileUrl]() {
        const auto localFile = fileUrl.toLocalFile();
        QFile file(localFile);
        if (!file.open(QFile::ReadOnly)) {
            messageService()->pushError(i18n("Failed to open save file"), file.errorString());
            QMetaObject::invokeMethod(mainWindow, &MainWindow::showLanding);
            return;
        }

        QMetaObject::invokeMethod(mainWindow, &MainWindow::saveRecentFile, fileUrl);
        Config::setLastOpenDir(fileUrl);
        Config::self()->save();

        Serializer serializer(&file);
        auto res = serializer.read<GameSave>();

        if (!res.has_value()) {
            messageService()->pushError(i18n("Failed to open save file"), res.error());
            QMetaObject::invokeMethod(mainWindow, &MainWindow::showLanding);
            return;
        }

        auto gameSave = std::move(res.value());
        gameSave.filePath = fileUrl;

        QMetaObject::invokeMethod(mainWindow, &MainWindow::showEditor, gameSave);
    });
}
} // namespace MEASE
