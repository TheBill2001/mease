// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "mease/ui/mainwindow.hpp"

#include <QApplication>
#include <QCommandLineParser>

#include <KAboutData>
#include <KIconTheme>
#include <KLocalizedString>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

using namespace Qt::Literals::StringLiterals;

int main(int argc, char **argv)
{
#ifdef Q_OS_WIN
    // If ran from a console, redirect the output there
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
#endif
    KIconTheme::initTheme();

    QApplication application(argc, argv);

#if defined(Q_OS_MACOS) || defined(Q_OS_WIN)
    QApplication::setStyle(u"breeze"_s);
#endif

    KLocalizedString::setApplicationDomain("mease");

    KAboutData aboutData(u"mease"_s,
                         i18n("Mass Effect: Andromeda Save Editor"),
                         QStringLiteral("0.0.1"),
                         i18n("Save editor for Mass Effect: Andromeda"),
                         KAboutLicense::GPL_V3,
                         i18n("Copyright (C) 2025, Trần Nam Tuấn <%1>", u"tuantran1632001@gmail.com"_s),
                         {},
                         u"https://github.com/TheBill2001/mease"_s,
                         u"https://github.com/TheBill2001/mease/issues"_s);

    aboutData.addAuthor(i18n("Trần Nam Tuấn"), i18n("Author"), u"tuantran1632001@gmail.com"_s);
    aboutData.setOrganizationDomain("thebill2001.github.io");
#if defined(Q_OS_UNIX)
    aboutData.setDesktopFileName(u"mease"_s);
#endif

    KAboutData::setApplicationData(std::move(aboutData));
    application.setWindowIcon(QIcon::fromTheme(u"mease"_s));

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);

    parser.process(application);
    aboutData.processCommandLine(&parser);

    auto *mainWindow = new MEASE::MainWindow(); // Declare on the stack cause invalid free
    mainWindow->show();

    return application.exec();
}
