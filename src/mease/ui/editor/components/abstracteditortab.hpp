// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_ABSTRACTEDITORTAB_HPP
#define MEASE_ABSTRACTEDITORTAB_HPP

#include <QWidget>

namespace MEASE
{
class AbstractEditorTab : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title CONSTANT FINAL)
protected:
    AbstractEditorTab(const QString &title, QWidget *parent = nullptr);

public:
    QString title() const;

private:
    QString m_title;
};
} // namespace MEASE

#endif // MEASE_ABSTRACTEDITORTAB_HPP
