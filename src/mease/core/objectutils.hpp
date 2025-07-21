// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef MEASE_OBJECTUTILS_HPP
#define MEASE_OBJECTUTILS_HPP

#include <qtclasshelpermacros.h>

#define MEASE_DECLARE_QT_PUBLIC(CLASS)                                                                                                                         \
    Q_DISABLE_COPY(CLASS##Private)                                                                                                                             \
    Q_DECLARE_PUBLIC(CLASS)                                                                                                                                    \
    CLASS *q_ptr;                                                                                                                                              \
                                                                                                                                                               \
    template<typename... Args>                                                                                                                                 \
    constexpr static auto connect(Args &&...args)                                                                                                              \
    {                                                                                                                                                          \
        return QObject::connect(std::forward<Args>(args)...);                                                                                                  \
    }

#endif // MEASE_OBJECTUTILS_HPP
