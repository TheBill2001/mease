#ifndef MEASE_MESSAGE_SERVICE_HPP
#define MEASE_MESSAGE_SERVICE_HPP

#include <QObject>

namespace MEASE
{

class MessageServicePrivate;
class MessageService final : public QObject
{
    Q_OBJECT
    friend class MessageServiceStatic;
    MessageService();

public:
    ~MessageService() override;

    void setMainWindow(QWidget *widget);

public Q_SLOTS:
    void pushError(const QString &title, const QString &message);

private:
    Q_DECLARE_PRIVATE(MessageService)
    MessageServicePrivate *d_ptr;
};

MessageService *messageService();

} // namespace MEASE

#endif // MEASE_MESSAGE_SERVICE_HPP
