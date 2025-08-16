# SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

add_library(mease_config STATIC)

kconfig_add_kcfg_files(mease_config config.kcfgc)

target_link_libraries(mease_config PUBLIC
    KF6::ConfigCore
    KF6::ConfigGui
)
