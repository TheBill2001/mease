// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_MAINWINDOW_HPP
#define MEASE_MAINWINDOW_HPP

#include <KXmlGuiWindow>

namespace MEASE
{
class GameSave;
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private Q_SLOTS:
    void saveRecentFile(const QUrl &fileUrl);

private:
    void showLanding();
    void showLoading();
    void showEditor(const GameSave &gameSave);
    void openSelectedFile(const QUrl &fileUrl);
};
} // namespace MEASE

#endif // MEASE_MAINWINDOW_HPP
