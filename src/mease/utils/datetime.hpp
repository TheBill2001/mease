// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_DATETIME_HPP
#define MEASE_DATETIME_HPP

#include <QDateTime>

namespace MEASE
{
QDateTime fromJulianSeconds(quint64 julianSeconds);
quint64 toJulianSeconds(const QDateTime &dateTime);
} // namespace MEASE

#endif // MEASE_DATETIME_HPP
