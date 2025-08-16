# SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

#!/bin/bash

set -e

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
SCRIPT_DIR="$(readlink -f "$SCRIPT_DIR")"

source "$SCRIPT_DIR/common.sh"

function show_help() {
    echo "Usage: build-qt.sh [OPTIONS]"
    echo "Building required Qt modules for MEASaveEditor"
    echo ""
    echo "Options:"
    echo "    -h, --help              show this help message"
    echo "    -p, --prefix            prefix path (default: $PREFIX)"
    echo "    -i, --install           installation path (default: $INSTALL_DIR)"
}

while [[ $# -gt 0 ]]; do
    case $1 in
        -p|--prefix)
            PREFIX="$2"
            shift
            shift
            ;;
        -i|--install)
            INSTALL_DIR="$2"
            shift
            shift
            ;;
        -h|--help)
            show_help
            shift
            exit 0
            ;;
        *)
            shift
            ;;
    esac
done

if [[ -z "$INSTALL_DIR" ]]; then
    echo "Missing install dir"
    exit 1
fi

PATH="$PATH:$PREFIX/bin"

case "$PLATFORM" in
    linux)
        LIB_PREFIX=lib
        LIB_SUFFIX=.so
        BIN_DIR="$INSTALL_DIR/bin"
        ;;
    windows)
        LIB_PREFIX=""
        LIB_SUFFIX=.dll
        BIN_DIR="$INSTALL_DIR"
        ;;
esac

PLUGIN_DIR="$INSTALL_DIR/plugins"
DATA_DIR="$BIN_DIR/data"
QT_DIR="$PREFIX"

##### Functions

function list_lib() {
    local -n detected_libs=$1

    case "$PLATFORM" in
        linux)
            mapfile -t detected_libs < <(ldd "$2" | grep -iv /usr | cut -d ' ' -f 3)
            ;;
        windows)
            mapfile -t detected_libs < <(ldd "$2" | grep -iv api-ms | grep -iv /c/windows | cut -d ' ' -f 3)
            ;;
    esac
}

function do_copy() {
    if ! [[ -f "$1" ]]; then
        echo "Not a file: $1"
        exit 1
    fi

    if ! [[ -d "$(dirname -- "$2")" ]]; then
        mkdir -p "$(dirname -- "$2")"
    fi

    if ! [[ -f "$2" ]]; then
        echo "-- Installing: $2"
        cp "$1" "$2"
    else
        if cmp -s "$1" "$2"; then
            echo "-- Up-to-date: $2"
        else
            echo "-- Updating: $2"
            cp -f "$1" "$2"
        fi
    fi
}

function copy_plugin() {
    local plugin="$1/${LIB_PREFIX}$2${LIB_SUFFIX}"

    do_copy "$QT_DIR/plugins/$plugin" "$PLUGIN_DIR/$plugin"

    list_lib libs "$QT_DIR/plugins/$plugin"
    for lib in "${libs[@]}"; do
        do_copy "$lib" "$BIN_DIR/$(basename -- "$lib")"
    done
}

function copy_kf_translation() {
    if [[ -f "$QT_DIR/bin/data/locale/$1/LC_MESSAGES/$2" ]]; then
        do_copy "$QT_DIR/bin/data/locale/$1/LC_MESSAGES/$2" "$BIN_DIR/data/locale/$1/LC_MESSAGES/$2"
    fi
}

##### Deploy

cmake --install "$PROJECT_DIR/build"

if [[ "$PLATFORM" == linux ]]; then
    echo "-- Writing $BIN_DIR/qt.conf"
    echo "[Paths]" > "$BIN_DIR/qt.conf"
    echo "Prefix = .." >> "$BIN_DIR/qt.conf"
fi

##### Main executable libs

case "$PLATFORM" in
    linux)
        list_lib libs "$PROJECT_DIR/build/bin/mease"
        ;;
    windows)
        list_lib libs "$PROJECT_DIR/build/bin/mease.exe"
        ;;
esac

for lib in "${libs[@]}"; do
    do_copy "$lib" "$BIN_DIR/$(basename -- "$lib")"
done

##### Qt plugins

echo "##### Installing Qt plugins"

for plugin in "${plugins[@]}"; do
    do_copy "$QT_DIR/plugins/$plugin" "$PLUGIN_DIR/$plugin"

    list_lib libs "$QT_DIR/plugins/$plugin"
    for lib in "${libs[@]}"; do
        do_copy "$lib" "$BIN_DIR/$(basename -- "$lib")"
    done
done

case "$PLATFORM" in
    linux)
        if [[ -f "$QT_DIR/plugins/libqxcb.so" ]]; then
            copy_plugin platforms qxcb
        fi

        if [[ -f "$QT_DIR/plugins/libqwayland-egl.so" ]]; then
            copy_plugin platforms qwayland-egl
        fi

        if [[ -f "$QT_DIR/plugins/libqwayland-generic.so" ]]; then
            copy_plugin platforms qwayland-generic
        fi

        if [[ -f "$QT_DIR/plugins/libqwayland.so" ]]; then
            copy_plugin platforms qwayland
        fi
        ;;
    windows)
        copy_plugin platforms qwindows
        ;;
esac

copy_plugin imageformats qsvg
copy_plugin styles breeze6
copy_plugin kiconthemes6/iconengines KIconEnginePlugin

##### Qt translations

echo "##### Installing Qt translations"

qt_translations=(
    en
)

for qt_translation in "${qt_translations[@]}"; do
    do_copy "$QT_DIR/translations/qt_$qt_translation.qm" "$INSTALL_DIR/translations/qt_$qt_translation.qm"
    do_copy "$QT_DIR/translations/qtbase_$qt_translation.qm" "$INSTALL_DIR/translations/qtbase_$qt_translation.qm"
done

##### KF translations

echo "##### Installing KDE translations"

kf_translations=(
    en
    en_GB
    vi
)

for kf_translation in "${kf_translations[@]}"; do
    if [[ -f "$QT_DIR/bin/data/locale/$kf_translation/kf6_entry.desktop" ]]; then
        do_copy "$QT_DIR/bin/data/locale/$kf_translation/kf6_entry.desktop" "$BIN_DIR/data/locale/$1/$kf_translation/kf6_entry.desktop"
    fi

    copy_kf_translation "$kf_translation" karchive6_qt.qm
    copy_kf_translation "$kf_translation" kcodecs6_qt.qm
    copy_kf_translation "$kf_translation" kcolorscheme6.mo
    copy_kf_translation "$kf_translation" kconfig6_qt.qm
    copy_kf_translation "$kf_translation" kconfigwidgets6.mo
    copy_kf_translation "$kf_translation" kcoreaddons6_qt.qm
    copy_kf_translation "$kf_translation" ki18n6.mo
    copy_kf_translation "$kf_translation" kiconthemes6.mo
    copy_kf_translation "$kf_translation" kitemviews6_qt.qm
    copy_kf_translation "$kf_translation" kwidgetsaddons6_qt.qm
    copy_kf_translation "$kf_translation" kwindowsystem6_qt.qm
    copy_kf_translation "$kf_translation" kxmlgui6.mo
done
