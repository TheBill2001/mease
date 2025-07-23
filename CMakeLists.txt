# SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.24)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)

set(KF6_MIN_VERSION 6.10)
set(QT6_MIN_VERSION 6.8.3)
set(ZLIB_MIN_VERSION 1.3.1)

set(CMAKE_AUTOUIC OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MEASaveEditor
    LANGUAGES CXX
    VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
    DESCRIPTION "Mass Effect: Andromeda Save Editor"
    HOMEPAGE_URL "https://github.com/TheBill2001/MEASaveEditor"
)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${ECM_MODULE_PATH}")

find_package(Qt6 ${QT6_MIN_VERSION} REQUIRED COMPONENTS Core Widgets LinguistTools)
find_package(ZLIB ${ZLIB_MIN_VERSION} REQUIRED)

include(ECMSetupVersion)
include(KDEClangFormat)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEGitCommitHooks)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX MEASE
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/mease/version.hpp"
)

set(lang_list
    en_US
)

qt_standard_project_setup(
    REQUIRES ${QT6_MIN_VERSION}
    I18N_TRANSLATED_LANGUAGES ${lang_list}
    I18N_SOURCE_LANGUAGE en_US
)

qt_add_executable(MEASaveEditor
    src/mease/main.cpp

    src/mease/data/savefile.cpp
    src/mease/data/savefile.hpp
    src/mease/data/savefileheader.cpp
    src/mease/data/savefileheader.hpp

    src/mease/misc/licenses/gpl-3.0-standalone.cpp
    src/mease/misc/licenses/gpl-3.0-standalone.hpp
    
    src/mease/misc/number/datetime.cpp
    src/mease/misc/number/datetime.hpp
    src/mease/misc/number/endian.hpp

    src/mease/serialize/datastream.cpp
    src/mease/serialize/savefile.cpp
    src/mease/serialize/savefileheader.cpp

    src/mease/services/message.cpp
    src/mease/services/message.hpp

    src/mease/ui/mainwindow.cpp
    src/mease/ui/mainwindow.hpp

    src/mease/ui/components/actionbutton.cpp
    src/mease/ui/components/actionbutton.hpp
    src/mease/ui/components/basetab.cpp
    src/mease/ui/components/basetab.hpp
    src/mease/ui/components/utils.cpp
    src/mease/ui/components/utils.hpp

    src/mease/ui/dialogs/aboutdialog.cpp
    src/mease/ui/dialogs/aboutdialog.hpp

    src/mease/ui/editor/rawdatatab.cpp
    src/mease/ui/editor/rawdatatab.hpp

    src/mease/ui/pages/editorpage.cpp
    src/mease/ui/pages/editorpage.hpp
    src/mease/ui/pages/landingpage.cpp
    src/mease/ui/pages/landingpage.hpp
    src/mease/ui/pages/loadingpage.cpp
    src/mease/ui/pages/loadingpage.hpp
)

foreach(lang ${lang_list})
    list(APPEND lang_sources "lang/MEASaveEditor_${lang}.ts")
endforeach()

qt_add_translations(MEASaveEditor
    TS_FILE_BASE MEASaveEditor
    TS_OUTPUT_DIRECTORY lang
)

target_include_directories(MEASaveEditor PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(MEASaveEditor PRIVATE
    Qt::Core
    Qt::Widgets
    ZLIB::ZLIB
)

target_compile_definitions(MEASaveEditor PRIVATE
    QT_DISABLE_DEPRECATED_UP_TO=0x060800
)

set_target_properties(MEASaveEditor PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

## Deployment
include(GNUInstallDirs)

install(TARGETS MEASaveEditor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION "${RUNTIME_INSTALL_DIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

set(deploy_tool_options
    "--translations en" # Qt's translations, not app's translation
)

if(WIN32)
    set(deploy_tool_options "${deploy_tool_options} --no-system-d3d-compiler --no-network --no-opengl-sw")
endif()

qt_generate_deploy_app_script(
    TARGET MEASaveEditor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_TOOL_OPTIONS ${deploy_tool_options}
)

install(SCRIPT ${deploy_script})

## Code Format
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.c src/*.cpp src/*.h src/*.hpp)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
