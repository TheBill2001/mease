// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "mease/utils/datetime.hpp"

namespace MEASE
{
namespace
{
constexpr auto unixSecJulianDelta = 210866803200;
}

QDateTime fromJulianSeconds(quint64 julianSeconds)
{
    const auto unixSeconds = julianSeconds - unixSecJulianDelta;

    const qint64 days = unixSeconds / 86400;
    const qint64 seconds = unixSeconds % 86400;

    return QDateTime::fromSecsSinceEpoch(0).addDays(days).addSecs(seconds);
}

quint64 toJulianSeconds(const QDateTime &dateTime)
{
    return (dateTime.isValid() ? dateTime.toSecsSinceEpoch() : 0) + unixSecJulianDelta;
}
} // namespace MEASE
