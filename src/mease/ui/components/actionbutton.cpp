#include "mease/ui/components/actionbutton.hpp"

namespace MEASE
{

ActionPushButton::ActionPushButton(QWidget *widget)
    : ActionPushButton{new QAction(this), widget}
{
}

ActionPushButton::ActionPushButton(QAction *action, QWidget *widget)
    : QPushButton{widget}
{
    setAction(action);
}

ActionPushButton::~ActionPushButton() = default;

QAction *ActionPushButton::action() const
{
    return m_action;
}

void ActionPushButton::setAction(QAction *action)
{
    if (action == m_action) {
        return;
    }

    if (m_action && m_action->parent() == this) {
        delete m_action;
    }

    m_action = action;

    if (m_action) {
        if (!m_action->parent()) {
            m_action->setParent(this);
        }

        setAutoRepeat(m_action->autoRepeat());
        setCheckable(m_action->isCheckable());
        setChecked(m_action->isChecked());
        setEnabled(m_action->isEnabled());
        setFont(m_action->font());
        setIcon(m_action->icon());
        setStatusTip(m_action->statusTip());
        setText(m_action->text());
        setToolTip(m_action->toolTip());
        setWhatsThis(m_action->whatsThis());

        connect(m_action, &QAction::changed, this, [this]() {
            setAutoRepeat(m_action->autoRepeat());
            setFont(m_action->font());
            setIcon(m_action->icon());
            setStatusTip(m_action->statusTip());
            setText(m_action->text());
            setToolTip(m_action->toolTip());
            setWhatsThis(m_action->whatsThis());
        });

        connect(m_action, &QAction::checkableChanged, this, &ActionPushButton::setCheckable);
        connect(m_action, &QAction::enabledChanged, this, &ActionPushButton::setEnabled);

        // connect(m_action, &QAction::toggled, this, &ActionPushButton::setChecked);
        // connect(this, &ActionPushButton::toggled, m_action, &QAction::setChecked);

        connect(this, &ActionPushButton::clicked, m_action, [this](bool checked) {
            m_action->setChecked(checked);
            m_action->trigger();
        });
    }

    Q_EMIT actionChanged(action);
}

} // namespace MEASE