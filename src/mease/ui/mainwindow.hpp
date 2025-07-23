// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_MAINWINDOW_HPP
#define MEASE_MAINWINDOW_HPP

#include <QMainWindow>

namespace MEASE
{
class SaveFileData;
class MainWindowPrivate;
class MainWindow final : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

Q_SIGNALS:
    void saveFileLoaded(const MEASE::SaveFileData &saveFiledata);
    void loadSaveFileFailed();

protected:
    void changeEvent(QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(MainWindow)
    QScopedPointer<MainWindowPrivate> d_ptr;

private Q_SLOTS:
    void updateActions();
    void openSaveFileActionTriggered();
    void openSaveFileSelected(const QString &filePath);
};
} // namespace MEASE
#endif // MEASE_MAINWINDOW_HPP
