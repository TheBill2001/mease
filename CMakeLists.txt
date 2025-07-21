# SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.24)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)

set(KF6_MIN_VERSION 6.13)
set(QT6_MIN_VERSION 6.8)

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

qt_standard_project_setup()

qt_add_executable(MEASaveEditor
    src/mease/main.cpp

    src/mease/core/objectutils.hpp

    src/mease/ui/aboutdialog.cpp
    src/mease/ui/aboutdialog.hpp
    src/mease/ui/mainwindow.cpp
    src/mease/ui/mainwindow.hpp
)

qt_add_translations(
    TARGETS MEASaveEditor
    TS_FILES
        lang/MEASaveEditor_en_US.ts
)

qt_add_resources(MEASaveEditor "resources"
    PREFIX "/MEASaveEditor"
    FILES
        res/text/gpl-3.0-standalone.html
)

target_include_directories(MEASaveEditor PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(MEASaveEditor PRIVATE
    Qt::Core
    Qt::Widgets
)

if(WIN32)
    set_target_properties(MEASaveEditor PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

if(APPLE)
    set_target_properties(MEASaveEditor PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()

## Deployment
include(GNUInstallDirs)

install(TARGETS MEASaveEditor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MEASaveEditor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})

## Code Format
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.c src/*.cpp src/*.h src/*.hpp)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
