#!/bin/bash

set -e

SOURCE_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )/.."
SOURCE_DIR="$(readlink -f "$SOURCE_DIR")"

PREFIX="$SOURCE_DIR/prefix"
QT_DIR=""
INSTALL_DIR="$SOURCE_DIR/install"

while [[ $# -gt 0 ]]; do
    case $1 in
        -p|--prefix)
            PREFIX="$2"
            shift
            shift
            ;;
        --qt-dir)
            QT_DIR="$2"
            shift
            shift
            ;;
        --install)
            INSTALL_DIR="$2"
            shift
            shift
            ;;
    esac
done

if [[ -z "$QT_DIR" ]]; then
    if ! [[ -f "$PREFIX/qt-root-dir"  ]]; then
        echo "Cannot determine Qt dir"
        exit 1
    fi

    QT_DIR="$PREFIX/$(cat "$PREFIX/qt-root-dir")"
fi

if [[ -z "$INSTALL_DIR" ]]; then
    echo "Missing install dir"
    exit 1
fi

IS_LINUX=0
IS_WINDOWS=0

case "$(uname -s)" in
    Linux*)
        IS_LINUX=1
        ;;
    MINGW64_NT*|MSYS_NT*)
        IS_WINDOWS=1
        ;;
    *)
        echo "UNSUPPORTED: $(uname -s)"
        exit 1
        ;;
esac

export PATH="$PATH:$QT_DIR/bin"

PLUGIN_DIR="$INSTALL_DIR/plugins"

if [[ "$IS_LINUX" -eq 1 ]]; then
    LIB_PREFIX=lib
    LIB_SUFFIX=.so
    BIN_DIR="$INSTALL_DIR/bin"
elif [[ "$IS_WINDOWS" -eq 1 ]]; then
    LIB_PREFIX=""
    LIB_SUFFIX=.dll
    BIN_DIR="$INSTALL_DIR"
fi

function list_lib() {
    local -n detected_libs=$1

    if [[ "$IS_LINUX" -eq 1 ]]; then
        mapfile -t detected_libs < <(ldd "$2" | grep -iv /usr | cut -d ' ' -f 3)
    elif [[ "$IS_WINDOWS" -eq 1 ]]; then
        mapfile -t detected_libs < <(ldd "$2" | grep -iv api-ms | grep -iv /c/windows | cut -d ' ' -f 3)
    fi
}

function do_copy() {
    if [[ -f "$2" ]]; then
        return
    fi

    echo "-- Installing: $1 => $2"
    ! [[ -d "$(dirname -- "$2")" ]] && mkdir -p "$(dirname -- "$2")"
    cp -f "$1" "$2"
}

cmake --install "$SOURCE_DIR/build"

# ! [[ -d "$BIN_DIR" ]] && mkdir -p "$BIN_DIR"
# echo "[Paths]" > "$BIN_DIR/qt.conf"
# echo "Prefix = .." >> "$BIN_DIR/qt.conf"

### Main executable libs

if [[ "$IS_LINUX" -eq 1 ]]; then
    list_lib libs "$SOURCE_DIR/build/bin/mease"
elif [[ "$IS_WINDOWS" -eq 1 ]]; then
    list_lib libs "$SOURCE_DIR/build/bin/mease.exe"
fi

for lib in "${libs[@]}"; do
    do_copy "$lib" "$BIN_DIR/$(basename -- "$lib")"
done

### Qt plugins

plugins=(
    "platforms/${LIB_PREFIX}qwindows${LIB_SUFFIX}"
    "imageformats/${LIB_PREFIX}qsvg${LIB_SUFFIX}"
    "styles/${LIB_PREFIX}breeze6${LIB_SUFFIX}"
    "kiconthemes6/iconengines/${LIB_PREFIX}KIconEnginePlugin${LIB_SUFFIX}"
)

for plugin in "${plugins[@]}"; do
    do_copy "$QT_DIR/plugins/$plugin" "$PLUGIN_DIR/$plugin"

    list_lib libs "$QT_DIR/plugins/$plugin"
    for lib in "${libs[@]}"; do
        do_copy "$lib" "$BIN_DIR/$(basename -- "$lib")"
    done
done

### Qt translations

qt_translations=(
    en
)

for qt_translation in "${qt_translations[@]}"; do
    do_copy "$QT_DIR/translations/qt_$qt_translation.qm" "$INSTALL_DIR/translations/qt_$qt_translation.qm"
    do_copy "$QT_DIR/translations/qtbase_$qt_translation.qm" "$INSTALL_DIR/translations/qtbase_$qt_translation.qm"
done

### KF translations

function copy_kf_translation() {
    if [[ -f "$QT_DIR/bin/data/locale/$kf_translations/LC_MESSAGES/$1" ]]; then
        do_copy "$QT_DIR/bin/data/locale/$kf_translations/LC_MESSAGES/$1" "$BIN_DIR/data/locale/$kf_translations/LC_MESSAGES/$1"
    fi
}

kf_translations=(
    en
    en_GB
    vi
)

for kf_translations in "${kf_translations[@]}"; do
    copy_kf_translation karchive6_qt.qm
    copy_kf_translation kcodecs6_qt.qm
    copy_kf_translation kcolorscheme6.mo
    copy_kf_translation kconfig6_qt.qm
    copy_kf_translation kconfigwidgets6.mo
    copy_kf_translation kcoreaddons6_qt.qm
    copy_kf_translation ki18n6.mo
    copy_kf_translation kiconthemes6.mo
    copy_kf_translation kitemviews6_qt.qm
    copy_kf_translation kwidgetsaddons6_qt.qm
    copy_kf_translation kwindowsystem6_qt.qm
    copy_kf_translation kxmlgui6.mo
done
