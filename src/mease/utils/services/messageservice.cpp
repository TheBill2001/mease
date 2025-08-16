// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "messageservice.hpp"

#include <QApplicationStatic>
#include <QQueue>

#include <KMessageDialog>

using namespace Qt::Literals::StringLiterals;

namespace MEASE
{
namespace
{
struct MessageInfo {
    KMessageDialog::Type type;
    QString caption;
    QString text;
    QString details;
};
}

class MessageServiceHelper
{
public:
    MessageService *const instance;

    MessageServiceHelper()
        : instance{new MessageService()}
    {
        instance->moveToThread(qApp->thread());
    }
};

Q_APPLICATION_STATIC(MessageServiceHelper, g_helper)

class MessageServicePrivate
{
    Q_DISABLE_COPY_MOVE(MessageServicePrivate)
    Q_DECLARE_PUBLIC(MessageService)
    MessageService *q_ptr;

    QQueue<MessageInfo> m_queue;
    KMessageDialog *m_dialog = nullptr;

public:
    mutable QMutex mutex;

    QWidget *mainWindow = nullptr;

    explicit MessageServicePrivate(MessageService *q_ptr)
        : q_ptr{q_ptr}
    {
    }

    bool isEmpty() const
    {
        QMutexLocker lock(&mutex);
        return m_queue.isEmpty();
    }

    void push(const MessageInfo &info)
    {
        {
            QMutexLocker lock(&mutex);
            m_queue.enqueue(info);
        }
        showMessage();
    }

    MessageInfo pop()
    {
        QMutexLocker lock(&mutex);
        return m_queue.dequeue();
    }

    void showMessage()
    {
        if (isEmpty()) {
            return;
        }

        {
            QMutexLocker lock(&mutex);
            if (m_dialog) {
                return;
            }
        }

        Q_Q(MessageService);

        auto message = pop();

        auto *dialog = new KMessageDialog(message.type, message.text);
        if (mainWindow) {
            dialog->moveToThread(mainWindow->thread());
            dialog->setParent(mainWindow);
        }
        dialog->setCaption(message.caption);
        dialog->setDetails(message.details);
        dialog->setAttribute(Qt::WA_DeleteOnClose, true);

        QObject::connect(dialog, &KMessageDialog::finished, q, [this]() {
            {
                QMutexLocker lock(&mutex);
                m_dialog = nullptr;
            }

            if (!isEmpty()) {
                showMessage();
            }
        });

        dialog->setWindowModality(Qt::WindowModal);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        switch (message.type) {
        case KMessageDialog::QuestionTwoActions:
        case KMessageDialog::QuestionTwoActionsCancel:
            dialog->setIcon(QIcon::fromTheme(u"dialog-question"_s));
            break;
        case KMessageDialog::WarningTwoActions:
        case KMessageDialog::WarningTwoActionsCancel:
        case KMessageDialog::WarningContinueCancel:
            dialog->setIcon(QIcon::fromTheme(u"dialog-warning"_s));
            break;
        case KMessageDialog::Information:
            dialog->setIcon(QIcon::fromTheme(u"dialog-information"_s));
            break;
        case KMessageDialog::Error:
            dialog->setIcon(QIcon::fromTheme(u"dialog-error"_s));
            dialog->setButtons();
            break;
        }

        {
            QMutexLocker lock(&mutex);
            m_dialog = dialog;
        }

        dialog->show();
    }
};

MessageService::MessageService()
    : d_ptr{new MessageServicePrivate(this)}
{
}

MessageService::~MessageService() = default;

QWidget *MessageService::mainWindow() const
{
    Q_D(const MessageService);
    QMutexLocker lock(&d->mutex);
    return d->mainWindow;
}

void MessageService::setMainWindow(QWidget *mainWindow)
{
    Q_D(MessageService);
    {
        QMutexLocker lock(&d->mutex);
        if (d->mainWindow == mainWindow) {
            return;
        }
        d->mainWindow = mainWindow;
    }
    Q_EMIT mainWindowChanged(d->mainWindow);
}

void MessageService::pushError(const QString &caption, const QString &message, const QString &details)
{
    Q_D(MessageService);
    d->push({.type = KMessageDialog::Error, .caption = caption, .text = message, .details = details});
}

MessageService *messageService()
{
    return g_helper->instance;
}
} // namespace MEASE
