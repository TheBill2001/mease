# SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.24)

project(MEASaveEditor
    LANGUAGES CXX
    VERSION 0.0.1
    DESCRIPTION "Mass Effect: Andromeda Save Editor"
    HOMEPAGE_URL "https://github.com/TheBill2001/MEASaveEditor"
)

set(QT_MIN_VERSION "6.8.0")
set(KF_MIN_VERSION "6.14.0")

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

# include(ECMInstallIcons)
# include(ECMQtDeclareLoggingCategory)
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

ecm_set_disabled_deprecation_versions(
    QT 6.8
)

add_subdirectory(src)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

## Code Format
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.c src/*.cpp src/*.h src/*.hpp)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
