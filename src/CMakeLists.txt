# SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

qt_add_executable(mease
    main.cpp

    mease/data/gamesave.cpp
    mease/data/gamesave.hpp
    mease/data/gamesaveheader.cpp
    mease/data/gamesaveheader.hpp

    mease/models/abstracttreeitem.cpp
    mease/models/abstracttreeitem.hpp
    mease/models/abstracttreemodel.cpp
    mease/models/abstracttreemodel.hpp
    mease/models/gamesaverawdataitem.cpp
    mease/models/gamesaverawdataitem.hpp
    mease/models/gamesaverawdatamodel.cpp
    mease/models/gamesaverawdatamodel.hpp
    mease/models/treeitem.cpp
    mease/models/treeitem.hpp

    mease/serialize/serializer.cpp
    mease/serialize/serializer.hpp
    mease/serialize/serializer_p.hpp

    mease/ui/mainwindow.cpp
    mease/ui/mainwindow.hpp
    mease/ui/units.cpp
    mease/ui/units.hpp

    mease/ui/editor/components/abstracteditortab.cpp
    mease/ui/editor/components/abstracteditortab.hpp
    mease/ui/editor/components/formeditortab.cpp
    mease/ui/editor/components/formeditortab.hpp
    mease/ui/editor/components/gamesaverawdataview.cpp
    mease/ui/editor/components/gamesaverawdataview.hpp
    mease/ui/editor/rawdatatab.cpp
    mease/ui/editor/rawdatatab.hpp

    mease/ui/pages/editorpage.cpp
    mease/ui/pages/editorpage.hpp
    mease/ui/pages/landingpage.cpp
    mease/ui/pages/landingpage.hpp
    mease/ui/pages/loadingpage.cpp
    mease/ui/pages/loadingpage.hpp

    mease/utils/datetime.cpp
    mease/utils/datetime.hpp
    mease/utils/macros.hpp
    mease/utils/number.hpp

    mease/utils/hashing
    mease/utils/hashing/djb2.hpp

    mease/utils/services/messageservice.cpp
    mease/utils/services/messageservice.hpp
)

add_subdirectory(mease)

target_include_directories(mease PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
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

    ZLIB::ZLIB

    mease_config
)

install(TARGETS mease ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/mease/ui/measeui.rc DESTINATION ${KDE_INSTALL_KXMLGUIDIR}/mease)
