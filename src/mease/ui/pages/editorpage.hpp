#ifndef MEASE_EDITOR_HPP
#define MEASE_EDITOR_HPP

#include <QWidget>

namespace MEASE
{

class EditorPage : public QWidget
{
    Q_OBJECT
public:
    EditorPage(QWidget *parent = nullptr);
};

} // namespace MEASE

#endif // MEASE_EDITOR_HPP
