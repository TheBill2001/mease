#ifndef MEASE_DATETIME_HPP
#define MEASE_DATETIME_HPP

#include <QDateTime>

namespace MEASE
{

QDateTime fromJulianSeconds(quint64 julianSeconds);
quint64 toJulianSeconds(const QDateTime &dateTime);

} // namespace MEASE

#endif // MEASE_DATETIME_HPP
