#include "mease/ui/editor/rawdatatab.hpp"

#include "mease/data/savefile.hpp"

namespace MEASE
{
class RawDataTabPrivate
{
    Q_DISABLE_COPY(RawDataTabPrivate)
    Q_DECLARE_PUBLIC(RawDataTab)
    RawDataTab *q_ptr;

public:
    SaveFile *saveFile;

    explicit RawDataTabPrivate(SaveFile *saveFile, RawDataTab *q_ptr)
        : q_ptr{q_ptr}
        , saveFile{saveFile}
    {
    }
};

RawDataTab::RawDataTab(SaveFile *saveFile, QWidget *widget)
    : BaseTab{widget}
    , d_ptr{new RawDataTabPrivate(saveFile, this)}
{
}

RawDataTab::~RawDataTab() = default;
} // namespace MEASE