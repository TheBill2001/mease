#ifndef MEASE_RAWDATATAB_HPP
#define MEASE_RAWDATATAB_HPP

#include "mease/ui/components/basetab.hpp"

namespace MEASE
{
class SaveFile;
class RawDataTabPrivate;
class RawDataTab : public BaseTab
{
    Q_OBJECT
public:
    explicit RawDataTab(SaveFile *saveFile, QWidget *widget = nullptr);
    ~RawDataTab() override;

public:
    Q_DECLARE_PRIVATE(RawDataTab)
    QScopedPointer<RawDataTabPrivate> d_ptr;
};
} // namespace MEASE

#endif // MEASE_RAWDATATAB_HPP
