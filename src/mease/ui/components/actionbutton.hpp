#ifndef MEASE_ACTIONBUTTON_HPP
#define MEASE_ACTIONBUTTON_HPP

#include <QPushButton>

namespace MEASE
{

class ActionPushButton final : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QAction *action READ action WRITE setAction NOTIFY actionChanged FINAL)
public:
    explicit ActionPushButton(QWidget *widget = nullptr);
    explicit ActionPushButton(QAction *action, QWidget *widget = nullptr);
    ~ActionPushButton() override;

    [[nodiscard]] QAction *action() const;
    void setAction(QAction *action);

Q_SIGNALS:
    void actionChanged(QAction *action);

private:
    QAction *m_action = nullptr;
};

} // namespace MEASE

#endif // MEASE_ACTIONBUTTON_HPP
