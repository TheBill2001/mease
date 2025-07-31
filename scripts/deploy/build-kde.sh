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
    echo "    -kv, --kf-version       KDE Framework version (default: $KF_VERSION)"
    echo "    -pv, --plasma-version   Plasma version (default: $PLASMA_VERSION)"
    echo "    -pwv, --plasma-wayland-protocols-version"
    echo "                            Plasma version (default: $PLASMA_WAYLAND_PROTOCOLS_VERSION)"
    echo "    -n, --no-clean          keep working files"
}

while [[ $# -gt 0 ]]; do
    case $1 in
        -p|--prefix)
            PREFIX="$(readlink -f "$2")"
            shift
            shift
            ;;
        -kv|--kf-version)
            KF_VERSION="$2"
            shift
            shift
            ;;
        -pv|--plasma-version)
            PLASMA_VERSION="$2"
            shift
            shift
            ;;
        -pwv|--plasma-wayland-protocols-version)
            PLASMA_WAYLAND_PROTOCOLS_VERSION="$2"
            shift
            shift
            ;;
        -n|--no-clean)
            KEEP=1
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

set_folders

echo "##### Info"
echo "-- Prefix: $PREFIX"
echo "-- Temp dir: $TEMP_DIR"

make_folders

set_python

function download_kde() {
    if ! [[ -d "$SRC_DIR/$1" ]]; then
        echo "-- Download $1"
        git clone --depth 1 -b "v$2" "https://github.com/KDE/$1.git" "$SRC_DIR/$1"
    fi
}

function download_kf() {
    download_kde "$1" "$KF_VERSION"
}

function download_plasma() {
    download_kde "$1" "$PLASMA_VERSION"
}

build() {
    local _name="$1"
    shift

    echo "##### Build $_name"

    if [[ -d "$SRC_DIR/$_name/build" ]]; then
        rm -rf "$SRC_DIR/$_name/build"
    fi

    cmake -S "$SRC_DIR/$_name" -B "$SRC_DIR/$_name/build" -G Ninja \
        -DBUILD_TESTING=OFF \
        -DCMAKE_BUILD_TYPE="Release" \
        -DCMAKE_INSTALL_PREFIX="$PREFIX" \
        -DCMAKE_PREFIX_PATH="$PREFIX" \
        "$@"

    cmake --build "$SRC_DIR/$_name/build" --parallel
    cmake --install "$SRC_DIR/$_name/build"
}

do_patch() {
    pushd "$SRC_DIR/$1"
    local _patch="$SCRIPT_DIR/patches/$2"
    OUT="$(patch -p1 --forward < "$_patch")" || echo "${OUT}" | grep "Skipping patch" -q || (echo "$OUT" && false)
    popd
}

echo "##### Downloads KDE stuffs"

PATH="$PATH:$PREFIX/bin"

download_kf kcoreaddons
download_kf ki18n
download_kf karchive
download_kf kwidgetsaddons
download_kf kconfig

if [[ "$PLATFORM" == linux ]]; then
    download_kde plasma-wayland-protocols "$PLASMA_WAYLAND_PROTOCOLS_VERSION"
fi

download_kf kguiaddons
download_kf kcolorscheme

download_kf breeze-icons

download_kf kiconthemes
download_kf kitemviews
download_kf kcodecs
download_kf kconfigwidgets
download_kf kxmlgui
download_kf kwindowsystem

download_plasma breeze
do_patch breeze "breeze.cmake.patch"

build kcoreaddons \
    -DKCOREADDONS_USE_QML=OFF \
    -DENABLE_INOTIFY=OFF \
    -DBUILD_PYTHON_BINDINGS=OFF \
    -DUSE_DBUS=OFF

build ki18n \
    -DPython3_ROOT_DIR="$PYTHON_PREFIX" \
    -DBUILD_WITH_QML=OFF \
    -DCMAKE_DISABLE_FIND_PACKAGE_IsoCodes=ON

build karchive \
    -DWITH_BZIP2=OFF \
    -DWITH_LIBLZMA=OFF \
    -DWITH_OPENSSL=OFF \
    -DWITH_LIBZSTD=OFF \
    -DCMAKE_DISABLE_FIND_PACKAGE_BZip2=ON \
    -DCMAKE_DISABLE_FIND_PACKAGE_LibLZMA=ON \
    -DCMAKE_DISABLE_FIND_PACKAGE_OpenSSL=ON \
    -DCMAKE_DISABLE_FIND_PACKAGE_PkgConfig=ON

build kwidgetsaddons \
    -DBUILD_PYTHON_BINDINGS=OFF \
    -DBUILD_DESIGNERPLUGIN=OFF

build kconfig \
    -DKCONFIG_USE_GUI=ON \
    -DKCONFIG_USE_QML=OFF \
    -DUSE_DBUS=OFF

if [[ "$PLATFORM" == linux ]]; then
    build plasma-wayland-protocols
fi

build kguiaddons \
    -DUSE_DBUS=OFF \
    -DBUILD_PYTHON_BINDINGS=OFF

build kcolorscheme

##### Breeze icons

WITH_ICON_GENERATION=OFF
if [[ "$PLATFORM" == linux ]]; then
    WITH_ICON_GENERATION=ON
fi

if [[ "$WITH_ICON_GENERATION" == ON ]]; then
    "$PYTHON" -m pip install -q lxml
fi

build breeze-icons \
    -DPython_ROOT_DIR="$PYTHON_PREFIX" \
    -DSKIP_INSTALL_ICONS=ON \
    -DWITH_ICON_GENERATION="$WITH_ICON_GENERATION" \
    -DBINARY_ICONS_RESOURCE=OFF

build kiconthemes \
    -DKICONTHEMES_USE_QTQUICK=OFF \
    -DBUILD_DESIGNERPLUGIN=OFF \
    -DUSE_DBUS=OFF \
    -DUSE_BreezeIcons=ON

build kitemviews \
    -DBUILD_DESIGNERPLUGIN=OFF

build kcodecs

build kconfigwidgets \
    -DBUILD_DESIGNERPLUGIN=OFF \
    -DUSE_DBUS=OFF

build kxmlgui \
    -DBUILD_DESIGNERPLUGIN=OFF \
    -DUSE_DBUS=OFF \
    -DBUILD_PYTHON_BINDINGS=OFF

build kwindowsystem \
    -DKWINDOWSYSTEM_QML=OFF

##### Breeze

build breeze \
    -DBUILD_QT5=OFF \
    -DWITH_DECORATIONS=OFF \
    -DWITH_WALLPAPERS=OFF \
    -DCMAKE_DISABLE_FIND_PACKAGE_Qt6Quick=ON

cleanup
