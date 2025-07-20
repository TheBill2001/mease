# SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

qt_add_executable(mease
    main.cpp

    mease/ui/mainwindow.cpp
    mease/ui/mainwindow.hpp
)

set_target_properties(mease PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(mease PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets

    KF6::CoreAddons
    KF6::I18n
    KF6::XmlGui
    KF6::IconThemes
)

if(NOT (LINUX OR (UNIX AND NOT APPLE)))
    target_link_libraries(mease PRIVATE
        Qt6::Svg
    )
endif()

if(LINUX OR (UNIX AND NOT APPLE))
    install(TARGETS mease ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
endif()

if(WIN32)
    install(TARGETS mease
        RUNTIME DESTINATION .
    )
endif()
