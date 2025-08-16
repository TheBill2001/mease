// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_MESSAGESERVICE_HPP
#define MEASE_MESSAGESERVICE_HPP

#include <QObject>

namespace MEASE
{
class MessageServicePrivate;
class MessageService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QWidget *mainWindow READ mainWindow WRITE setMainWindow NOTIFY mainWindowChanged FINAL)

    Q_DISABLE_COPY_MOVE(MessageService)

    friend class MessageServiceHelper;
    MessageService();

public:
    ~MessageService() override;

    QWidget *mainWindow() const;
    void setMainWindow(QWidget *mainWindow);

public Q_SLOTS:
    void pushError(const QString &caption, const QString &message, const QString &details = {});

Q_SIGNALS:
    void mainWindowChanged(QWidget *mainWindow);

private:
    Q_DECLARE_PRIVATE(MessageService)
    QScopedPointer<MessageServicePrivate> d_ptr;
};

MessageService *messageService();
} // namespace MEASE

#endif // MEASE_MESSAGESERVICE_HPP
