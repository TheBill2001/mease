#ifndef MEASE_EDITOR_HPP
#define MEASE_EDITOR_HPP

#include <QTabWidget>

namespace MEASE
{
class SaveFileData;
class EditorPagePrivate;
class EditorPage final : public QTabWidget
{
    Q_OBJECT
public:
    explicit EditorPage(QWidget *parent = nullptr);
    ~EditorPage() override;

public Q_SLOTS:
    void loadSaveFile(const SaveFileData &saveFileData);

protected:
    void changeEvent(QEvent *event) override;

private:
    Q_DECLARE_PRIVATE(EditorPage)
    QScopedPointer<EditorPagePrivate> d_ptr;
};
} // namespace MEASE

Q_MOC_INCLUDE("mease/data/savefile.hpp")

#endif // MEASE_EDITOR_HPP
