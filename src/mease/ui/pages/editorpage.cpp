#include "editorpage.hpp"

namespace MEASE
{
EditorPage::EditorPage(QWidget *parent)
    : KPageWidget{parent}
{
    setFaceType(Tabbed);
}
} // namespace MEASE
