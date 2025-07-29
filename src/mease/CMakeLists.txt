add_library(mease_config STATIC)

kconfig_add_kcfg_files(mease_config config.kcfgc)

target_link_libraries(mease_config PUBLIC
    KF6::ConfigCore
    KF6::ConfigGui
)
