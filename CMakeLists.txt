# SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.24)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)

set(KF6_MIN_VERSION 6.13)
set(QT6_MIN_VERSION 6.9)

set(CMAKE_AUTOUIC OFF)

project(MEASaveEditor
    LANGUAGES CXX
    VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
    DESCRIPTION "Mass Effect: Andromeda Save Editor"
    HOMEPAGE_URL "https://github.com/TheBill2001/MEASaveEditor"
)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${ECM_MODULE_PATH}")

find_package(Qt6 ${QT6_MIN_VERSION} REQUIRED COMPONENTS Core Widgets LinguistTools)

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

    src/mease/core/objectutils.hpp

    src/mease/misc/licenses/gpl-3.0-standalone.cpp
    src/mease/misc/licenses/gpl-3.0-standalone.hpp

    src/mease/ui/aboutdialog.cpp
    src/mease/ui/aboutdialog.hpp
    src/mease/ui/mainwindow.cpp
    src/mease/ui/mainwindow.hpp
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

list(JOIN lang_list "," deploy_lang)
set(deploy_tool_options "--translations ${deploy_lang}")

if(WIN32)
    set(deploy_tool_options "${deploy_tool_options} --no-system-d3d-compiler --no-network --no-opengl-sw")
endif()

install(TARGETS MEASaveEditor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION "${RUNTIME_INSTALL_DIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

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
