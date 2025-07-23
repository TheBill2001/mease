// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "mease/ui/mainwindow.hpp"
#include "mease/version.hpp"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QtSystemDetection>

using namespace Qt::Literals::StringLiterals;

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    QApplication::setStyle(u"fusion"_s);
#elif defined(Q_OS_MACOS)
    QApplication::setStyle(u"macos"_s);
#endif

    QApplication app(argc, argv);
    app.setApplicationName(u"MEASaveEditor"_s);
    app.setApplicationDisplayName(QObject::tr("Mass Effect: Andromeda Save Editor"));
    app.setApplicationVersion(QStringLiteral(MEASE_VERSION_STRING));
    app.setOrganizationName(u"TheBill2001"_s);
    app.setOrganizationDomain(u"thebill2001.github.io"_s);
    // app.setWindowIcon();
#ifdef Q_OS_UNIX
    // app.setDesktopFileName();
#endif

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = u"MEASaveEditor_"_s + QLocale(locale).name();
        if (translator.load(u":/i18n/"_s + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    MEASE::MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
