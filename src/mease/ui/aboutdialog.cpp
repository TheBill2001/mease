#include "mease/ui/aboutdialog.hpp"

#include "mease/core/objectutils.hpp"
#include "mease/misc/licenses/gpl-3.0-standalone.hpp"

#include <QApplication>
#include <QDialogButtonBox>
#include <QEvent>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QTabWidget>
#include <QVBoxLayout>

#define ICON_SIZE 75

using namespace Qt::Literals::StringLiterals;

namespace MEASE
{

class AboutDialogPrivate
{
    MEASE_DECLARE_QT_PUBLIC(AboutDialog)
public:
    QTabWidget *tabWidget = nullptr;
    QLabel *aboutLabel = new QLabel();
    QLabel *authorsLabel = new QLabel();
    // QLabel *specialThanksLabel = new QLabel();
    QLabel *licenseLabel = new QLabel();
    QLabel *softwareUsedLabel = new QLabel();

    AboutDialogPrivate(AboutDialog *self)
        : q_ptr{self}
        , tabWidget{new QTabWidget(self)}
    {
        auto *layout = new QVBoxLayout(self);
        self->setLayout(layout);
        layout->addWidget(tabWidget);

        newTab(AboutDialog::tr("About"), aboutLabel, QIcon::fromTheme(QIcon::ThemeIcon::ImageMissing));
        newTab(AboutDialog::tr("Authors"), authorsLabel);
        // newTab(AboutDialog::tr("Special Thanks"), specialThanksLabel);
        newTab(AboutDialog::tr("License"), licenseLabel);
        newTab(AboutDialog::tr("Software Used"), softwareUsedLabel);
        softwareUsedLabel->setMinimumWidth(300);

        {
            licenseLabel->setText(gpl30StandaloneText);
        }

        retranslateUi();
    }

    void newTab(const QString &title, QLabel *text, const QIcon &icon = {}, Qt::TextFormat format = Qt::RichText)
    {
        auto *content = new QWidget();
        auto *layout = new QHBoxLayout(content);

        layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));

        if (!icon.isNull()) {
            auto *iconLabel = new QLabel();
            iconLabel->setPixmap(icon.pixmap(ICON_SIZE));
            iconLabel->setFixedSize(ICON_SIZE, ICON_SIZE);
            layout->addWidget(iconLabel, 0, Qt::AlignTop);
        }

        {
            text->setTextFormat(format);
            text->setWordWrap(true);
            text->setOpenExternalLinks(true);
            text->setTextInteractionFlags(Qt::TextBrowserInteraction);
            layout->addWidget(text, 0, Qt::AlignTop);
        }

        layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));

        auto *scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(content);

        tabWidget->addTab(scrollArea, title);
    }

    void retranslateUi()
    {
        Q_Q(AboutDialog);
        q->setWindowTitle(AboutDialog::tr("About"));

        tabWidget->setTabText(0, AboutDialog::tr("About"));
        tabWidget->setTabText(1, AboutDialog::tr("Authors"));

        {
            QStringList list;
            list << u"<h3>%1 v%2</h3>"_s.arg(AboutDialog::tr("About Mass Effect: Andromeda Save Editor"), qApp->applicationVersion());
            list << AboutDialog::tr("<p>A save editor for Mass Effect: Andromeda Save Editor.</p>");
            list << AboutDialog::tr("<p>Copyright © 2025 Trần Nam Tuấn</p>");
            list << u"<table style=\"margin-top: 16px\">"_s;
            list << u"<tr>"_s;
            list << u"<td>%1:</td><td><a href=\"%2\">%2</a></td>"_s.arg(AboutDialog::tr("Homepage"), u"https://github.com/TheBill2001/MEASaveEditor"_s);
            list << u"</tr>"_s;
            list << u"<tr>"_s;
            list << u"<td>%1:</td><td><a href=\"%2\">%2</a></td>"_s.arg(AboutDialog::tr("Bug report"),
                                                                        u"https://github.com/TheBill2001/MEASaveEditor/issues"_s);
            list << u"</tr>"_s;
            list << u"</table>"_s;

            aboutLabel->setText(list.join(u"\n"_s));
        }

        {
            QStringList list;
            list << u"<h3>%1</h3>"_s.arg(AboutDialog::tr("Author and maintainer"));
            list << u"<table>"_s;
            list << u"<tr>"_s;
            list << u"<td>%1:</td><td>%2</td>"_s.arg(AboutDialog::tr("Name", "People's name"), u"Trần Nam Tuấn"_s);
            list << u"</tr>"_s;
            list << u"<tr>"_s;
            list << u"<td>%1:</td><td><a href=\"%2\">%2</a></td>"_s.arg(AboutDialog::tr("Website"), u"https://github.com/TheBill2001"_s);
            list << u"</tr>"_s;
            list << u"<tr>"_s;
            list << u"<td>%1:</td><td><a href=\"mailto:%2\">%2</a></td>"_s.arg(AboutDialog::tr("E-mail"), u"tuantran1632001@gmail.com"_s);
            list << u"</tr>"_s;
            list << u"</table>"_s;

            authorsLabel->setText(list.join(u"\n"_s));
        }

        // {
        //     specialThanksLabel->setText(u"WIP"_s);
        // }

        {
            QStringList list;
            list << u"<p>%1:</p>"_s.arg(
                AboutDialog::tr("%1 was built with the following libraries").arg(u"<b>%1</b>"_s.arg(AboutDialog::tr("Mass Effect: Andromeda Save Editor"))));
            list << u"<table style=\"margin-left: 16px;\">"_s;
            list << u"<tr>"_s;
            list << u"<td><a href=\"https://www.qt.io/\">Qt</a>:</td><td>%2</td>"_s.arg(QStringLiteral(QT_VERSION_STR));
            list << u"</tr>"_s;
            // list << u"<tr>"_s;
            // list << u"<td>zlib:</td><td>%2</td>"_s.arg(QStringLiteral(""));
            // list << u"</tr>"_s;
            list << u"</table>"_s;

            softwareUsedLabel->setText(list.join(u"\n"_s));
        }
    }
};

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog{parent}
    , d_ptr{new AboutDialogPrivate(this)}
{
    setMinimumSize(500, 200);
}

AboutDialog::~AboutDialog() = default;

void AboutDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        Q_D(AboutDialog);
        d->retranslateUi();
    }

    QWidget::changeEvent(event);
}

} // namespace MEASE