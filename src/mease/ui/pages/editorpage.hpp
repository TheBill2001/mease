#ifndef MEASE_EDITORPAGE_HPP
#define MEASE_EDITORPAGE_HPP

#include <KPageWidget>

namespace MEASE
{
class EditorPage : public KPageWidget
{
    Q_OBJECT
public:
    explicit EditorPage(QWidget *parent = nullptr);
};
} // namespace MEASE

#endif // MEASE_EDITORPAGE_HPP
