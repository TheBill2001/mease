#include "mease/services/message.hpp"

#include <QApplicationStatic>
#include <QMessageBox>
#include <QMutex>
#include <QQueue>

namespace MEASE
{

class MessageServiceStatic
{
    Q_DISABLE_COPY_MOVE(MessageServiceStatic)
public:
    MessageService *const service;

    MessageServiceStatic()
        : service{new MessageService()}
    {
        service->moveToThread(qApp->thread());
    }
};

Q_APPLICATION_STATIC(MessageServiceStatic, g_static)

struct MessageInfo {
    QMessageBox::Icon icon;
    QString title;
    QString message;
};

class MessageServicePrivate : public QObject
{
    Q_OBJECT
public:
    MessageServicePrivate(MessageService *parent)
        : QObject{parent}
    {
        connect(this, &MessageServicePrivate::messagePushed, this, &MessageServicePrivate::showMessage);
    }

    void pushMessage(const MessageInfo &message)
    {
        QMutexLocker lock(&m_mutex);
        m_messageQueue.enqueue(message);
        Q_EMIT messagePushed();
    }

    void setMainWindow(QWidget *widget)
    {
        QMutexLocker lock(&m_mutex);
        m_mainWindow = widget;
    }

Q_SIGNALS:
    void messagePushed();

private:
    mutable QMutex m_mutex;
    QQueue<MessageInfo> m_messageQueue;
    QWidget *m_mainWindow = nullptr;
    QMessageBox *m_messageBox = nullptr;

    bool hasMessage() const
    {
        QMutexLocker lock(&m_mutex);
        return !m_messageQueue.empty();
    }

    MessageInfo popMessage()
    {
        QMutexLocker lock(&m_mutex);
        return m_messageQueue.dequeue();
    }

    bool hasMessageDialog()
    {
        QMutexLocker lock(&m_mutex);
        return m_messageBox;
    }

private Q_SLOTS:
    void showMessage()
    {
        if (hasMessageDialog() || !hasMessage()) {
            return;
        }

        const auto message = popMessage();
        auto *messageBox = new QMessageBox(message.icon, message.title, message.message, QMessageBox::Close, m_mainWindow);
        connect(messageBox, &QMessageBox::finished, this, &MessageServicePrivate::closeMessageDialog);

        showMessageDialog(messageBox);
    }

    void showMessageDialog(QMessageBox *messageBox)
    {
        QMutexLocker lock(&m_mutex);
        m_messageBox = messageBox;
        m_messageBox->show();
    }

    void closeMessageDialog()
    {
        QMutexLocker lock(&m_mutex);
        m_messageBox->deleteLater();
        m_messageBox = nullptr;
    }
};

MessageService::MessageService()
    : d_ptr{new MessageServicePrivate(this)}
{
}

MessageService::~MessageService() = default;

void MessageService::setMainWindow(QWidget *widget)
{
    Q_D(MessageService);
    d->setMainWindow(widget);
}

void MessageService::pushError(const QString &title, const QString &message)
{
    Q_D(MessageService);
    d->pushMessage({QMessageBox::Icon::Critical, title, message});
}

MessageService *messageService()
{
    return g_static->service;
}

} // namespace MEASE

#include "message.moc"