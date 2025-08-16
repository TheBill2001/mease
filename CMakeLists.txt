# SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.24)

project(mease
    LANGUAGES CXX
    VERSION 0.0.1
    DESCRIPTION "Mass Effect: Andromeda Save Editor"
    HOMEPAGE_URL "https://github.com/TheBill2001/mease"
)

set(QT_MIN_VERSION "6.8.0")
set(KF_MIN_VERSION "6.14.0")
set(ZLIB_MIN_VERSION 1.3.1)

set(CMAKE_CXX_STANDARD 23)

find_package(ECM ${KF_MIN_VERSION} REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

if(WIN32)
    set(CMAKE_INSTALL_BINDIR .)
endif()

include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEClangFormat)
include(KDEGitCommitHooks)

include(ECMInstallIcons)
include(ECMDeprecationSettings)

include(FeatureSummary)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core
    Gui
    Widgets
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons
    Config
    I18n
    XmlGui
    IconThemes
    WidgetsAddons
)

find_package(ZLIB ${ZLIB_MIN_VERSION} REQUIRED)

ecm_set_disabled_deprecation_versions(
    QT 6.8
)

add_subdirectory(src)

add_custom_target(mease_translate
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/translate/messages.sh"
        --project-name "${CMAKE_PROJECT_NAME}"
        --project-version "${CMAKE_PROJECT_VERSION}"
        --bug-report "${CMAKE_PROJECT_HOMEPAGE_URL}/issues"
)

ki18n_install(lang_po)

ecm_install_icons(ICONS "${CMAKE_CURRENT_SOURCE_DIR}/icons/sc-apps-mease.svg" DESTINATION ${KDE_INSTALL_ICONDIR})

if(WIN32)
    set(mease_exe_icon "${CMAKE_CURRENT_SOURCE_DIR}/icons/mease.ico")

    configure_file(mease.rc.in "${CMAKE_CURRENT_BINARY_DIR}/mease.rc" @ONLY)
    target_sources(mease PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/mease.rc")
endif()

configure_file(mease.desktop.in "${CMAKE_CURRENT_BINARY_DIR}/mease.desktop" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/mease.desktop" DESTINATION "${KDE_INSTALL_APPDIR}")

install(FILES mimetype/mease.xml DESTINATION "${KDE_INSTALL_MIMEDIR}")

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

## Code Format
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.hpp)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
