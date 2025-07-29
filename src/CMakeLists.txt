# SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(mease
    main.cpp

    mease/ui/mainwindow.cpp
    mease/ui/mainwindow.hpp

    mease/ui/pages/editorpage.cpp
    mease/ui/pages/editorpage.hpp
    mease/ui/pages/landingpage.cpp
    mease/ui/pages/landingpage.hpp
    mease/ui/pages/loadingpage.cpp
    mease/ui/pages/loadingpage.hpp
)

add_subdirectory(mease)

target_include_directories(mease PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}"
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
    KF6::WidgetsAddons

    mease_config
)

target_link_libraries(mease PRIVATE Qt6::Widgets)

install(TARGETS mease ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/mease/ui/measeui.rc DESTINATION ${KDE_INSTALL_KXMLGUIDIR}/mease)
