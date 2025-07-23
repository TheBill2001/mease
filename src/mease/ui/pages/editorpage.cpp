#include "mease/ui/pages/editorpage.hpp"

#include "mease/data/savefile.hpp"
#include "mease/ui/editor/rawdatatab.hpp"

#include <QEvent>

namespace MEASE
{
class EditorPagePrivate final
{
    Q_DISABLE_COPY(EditorPagePrivate)
    Q_DECLARE_PUBLIC(EditorPage)
    EditorPage *q_ptr;

public:
    SaveFile *saveFile = nullptr;
    RawDataTab *rawdatatab = nullptr;

    explicit EditorPagePrivate(EditorPage *q_ptr)
        : q_ptr{q_ptr}
    {
    }

    void retranslateUi()
    {
        Q_Q(EditorPage);
        q->setTabText(q->indexOf(rawdatatab), EditorPage::tr("Raw Data", "Editor tab title"));
    }
};

EditorPage::EditorPage(QWidget *parent)
    : QTabWidget{parent}
    , d_ptr{new EditorPagePrivate(this)}
{
}

EditorPage::~EditorPage() = default;

void EditorPage::loadSaveFile(const SaveFileData &saveFileData)
{
    Q_D(EditorPage);
    if (d->saveFile) {
        delete d->saveFile;
        delete d->rawdatatab;

        d->saveFile = nullptr;
        d->rawdatatab = nullptr;
    }

    d->saveFile = new SaveFile(saveFileData, this);
    d->rawdatatab = new RawDataTab(d->saveFile);

    addTab(d->rawdatatab, {});

    d->retranslateUi();
}

void EditorPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        Q_D(EditorPage);
        d->retranslateUi();
    }
    QWidget::changeEvent(event);
}
} // namespace MEASE