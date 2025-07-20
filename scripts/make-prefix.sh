#!/bin/bash

set -e

SOURCE_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )/.."
SOURCE_DIR="$(readlink -f "$SOURCE_DIR")"
PREFIX="$SOURCE_DIR/prefix"

QT_VERSION=6.9.1
QT_DIR=""

KF_VERSION=6.16.0
PLASMA_VERSION=6.4.3
CLEAN_UP=0
DEBUG=0

while [[ $# -gt 0 ]]; do
    case $1 in
        -p|--prefix)
            PREFIX="$2"
            shift
            shift
            ;;
        --qt-version)
            QT_VERSION="$2"
            shift
            shift
            ;;
        --qt-dir)
            QT_DIR="$2"
            shift
            shift
            ;;
        --kf-version)
            KF_VERSION="$2"
            shift
            shift
            ;;
        --plasma-version)
            PLASMA_VERSION="$2"
            shift
            shift
            ;;
        --clean-up)
            CLEAN_UP=1
            shift
            ;;
        -d|--debug)
            DEBUG=1
            shift
            ;;
    esac
done

TEMP_DIR="$PREFIX/temp"
mkdir -p "$TEMP_DIR"

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

if ! [[ $(type -P python3) ]]; then
    echo "Python 3 is not installed"
    exit 1
fi

echo "######### Setting up Python environment"

if [[ "$IS_LINUX" -eq 1 ]]; then
    if ! [[ -d "$TEMP_DIR/venv" ]]; then
        python3 -m venv "$TEMP_DIR/venv"
    fi

    source "$TEMP_DIR/venv/bin/activate"
    PYTHON="$(type -P python3)"
    deactivate
elif [[ "$IS_WINDOWS" -eq 1 ]]; then
    PYTHON="$(type -P python3)"
fi

PYTHON_PREFIX="$("$PYTHON" -c "import sys; print(sys.prefix)")"

echo "- Python executable: '$PYTHON'"
echo "- Python install dir: '$PYTHON_PREFIX'"

if [[ -z "$QT_DIR" ]]; then
    echo "######### Install Qt $QT_VERSION"

    MODULES=()
    if [[ "$DEBUG" -eq 1 ]]; then
        MODULES=(-m debug_info)
    fi

    if [[ "$IS_LINUX" -eq 1 ]]; then
        if ! [[ -d "$PREFIX/$QT_VERSION/gcc_64" ]]; then
            echo "- Install aqtinstall"
            "$PYTHON" -m pip install --ignore-installed -q aqtinstall

            echo "- Install Qt to '$PREFIX'"
            "$PYTHON" -m aqt install-qt -O "$PREFIX" linux desktop "$QT_VERSION" linux_gcc_64 "${MODULES[@]}"
        fi

        QT_DIR="$QT_VERSION/gcc_64"

    elif [[ "$IS_WINDOWS" -eq 1 ]]; then
        if ! [[ -f "$TEMP_DIR/aqt.exe" ]]; then
            curl -JL "https://github.com/miurahr/aqtinstall/releases/download/v3.3.0/aqt_x64.exe" -o "$TEMP_DIR/aqt.exe"
        fi

        if ! [[ -d "$PREFIX/$QT_VERSION/llvm-mingw_64" ]]; then
            echo "- Install Qt to '$PREFIX'"
            "$TEMP_DIR/aqt.exe" install-qt -O "$PREFIX" windows desktop "$QT_VERSION" win64_llvm_mingw "${MODULES[@]}"
        fi

        QT_DIR="$QT_VERSION/llvm-mingw_64"
    fi
fi

echo "$QT_DIR" > "$PREFIX/qt-root-dir"
QT_DIR="$PREFIX/$QT_DIR"

export PATH="$PATH:$QT_DIR/bin"

CC="$(type -P clang)"
export CC

CXX="$(type -P clang++)"
export CXX

build() {
    local SOURCE_DIR="$1"

    local BUILD_TYPE="Release"
    if [[ "$DEBUG" -eq 1 ]]; then
        BUILD_TYPE="Debug"
    fi

    cmake --fresh -S "$SOURCE_DIR" -B "$SOURCE_DIR/build" -G Ninja \
        -DBUILD_TESTING=OFF \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DCMAKE_INSTALL_PREFIX="$QT_DIR" \
        -DCMAKE_PREFIX_PATH="$QT_DIR" \
        "$@"

    cmake --build "$SOURCE_DIR/build" --parallel
    cmake --install "$SOURCE_DIR/build"
}

kf_build() {
    local SOURCE_DIR="$TEMP_DIR/$1"
    local GIT_SOURCE="$2"
    shift 2

    if ! [[ -d "$SOURCE_DIR" ]]; then
        git clone -b "v$KF_VERSION" --depth=1 "$GIT_SOURCE" "$SOURCE_DIR"
    fi

    build "$SOURCE_DIR" "$@"
}

plasma_build() {
    local SOURCE_DIR="$TEMP_DIR/$1"
    local GIT_SOURCE="$2"
    shift 2

    if ! [[ -d "$SOURCE_DIR" ]]; then
        git clone -b "v$PLASMA_VERSION" --depth=1 "$GIT_SOURCE" "$SOURCE_DIR"
    fi

    build "$SOURCE_DIR" "$@"
}

do_patch() {
    OUT="$(patch -p1 --forward < "$1")" || echo "${OUT}" | grep "Skipping patch" -q || (echo "$OUT" && false);
}

echo "######### Build KCoreAddons"

kf_build "kcoreaddons" "https://github.com/KDE/kcoreaddons" \
    -DKCOREADDONS_USE_QML=OFF \
    -DENABLE_INOTIFY=OFF \
    -DBUILD_PYTHON_BINDINGS=OFF \
    -DUSE_DBUS=OFF

echo "######### Build KI18n"

kf_build "ki18n" "https://github.com/KDE/ki18n" \
    -DPython3_ROOT_DIR="$PYTHON_PREFIX" \
    -DBUILD_WITH_QML=OFF \
    -DCMAKE_DISABLE_FIND_PACKAGE_IsoCodes=ON

echo "######### Build KArchive"

kf_build "karchive" "https://github.com/KDE/karchive" \
    -DWITH_BZIP2=OFF \
    -DWITH_LIBLZMA=OFF \
    -DWITH_OPENSSL=OFF \
    -DWITH_LIBZSTD=OFF \
    -DCMAKE_DISABLE_FIND_PACKAGE_BZip2=ON \
    -DCMAKE_DISABLE_FIND_PACKAGE_LibLZMA=ON \
    -DCMAKE_DISABLE_FIND_PACKAGE_OpenSSL=ON \
    -DCMAKE_DISABLE_FIND_PACKAGE_PkgConfig=ON

echo "######### Build KWidgetsAddons"

kf_build "kwidgetsaddons" "https://github.com/KDE/kwidgetsaddons" \
    -DBUILD_PYTHON_BINDINGS=OFF \
    -DBUILD_DESIGNERPLUGIN=OFF

echo "######### Build KConfig"

kf_build "kconfig" "https://github.com/KDE/kconfig" \
    -DKCONFIG_USE_GUI=ON \
    -DKCONFIG_USE_QML=OFF \
    -DUSE_DBUS=OFF

echo "######### Build KGuiAddons"

kf_build "kguiaddons" "https://github.com/KDE/kguiaddons" \
    -DUSE_DBUS=OFF \
    -DBUILD_PYTHON_BINDINGS=OFF

echo "######### Build KColorScheme"

kf_build "kcolorscheme" "https://github.com/KDE/kcolorscheme" \

echo "######### Build Breeze Icons"

WITH_ICON_GENERATION=OFF
if [[ "$IS_LINUX" -eq 1 ]]; then
    WITH_ICON_GENERATION=ON
fi

if [[ "$WITH_ICON_GENERATION" = "ON" ]]; then
    "$PYTHON" -m pip install -q lxml
fi

kf_build "breeze-icons" "https://github.com/KDE/breeze-icons" \
    -DPython_ROOT_DIR="$PYTHON_PREFIX" \
    -DSKIP_INSTALL_ICONS=ON \
    -DWITH_ICON_GENERATION="$WITH_ICON_GENERATION" \
    -DBINARY_ICONS_RESOURCE=OFF

echo "######### Build KIconThemes"

kf_build "kiconthemes" "https://github.com/KDE/kiconthemes" \
    -DKICONTHEMES_USE_QTQUICK=OFF \
    -DBUILD_DESIGNERPLUGIN=OFF \
    -DUSE_DBUS=OFF \
    -DUSE_BreezeIcons=ON

echo "######### Build KItemViews"

kf_build "kitemviews" "https://github.com/KDE/kitemviews" \
    -DBUILD_DESIGNERPLUGIN=OFF

echo "######### Build KCodecs"

kf_build "kcodecs" "https://github.com/KDE/kcodecs"

echo "######### Build KConfigWidgets"

kf_build "kconfigwidgets" "https://github.com/KDE/kconfigwidgets" \
    -DBUILD_DESIGNERPLUGIN=OFF \
    -DUSE_DBUS=OFF

echo "######### Build KXmlGui"

kf_build "kxmlgui" "https://github.com/KDE/kxmlgui" \
    -DBUILD_DESIGNERPLUGIN=OFF \
    -DUSE_DBUS=OFF \
    -DBUILD_PYTHON_BINDINGS=OFF

echo "######### Build KWindowSystem"

kf_build "kwindowsystem" "https://github.com/KDE/kwindowsystem" \
    -DKWINDOWSYSTEM_QML=OFF

echo "######### Build Breeze"

if ! [[ -d "$TEMP_DIR/breeze" ]]; then
    git clone -b "v$PLASMA_VERSION" --depth=1 "https://github.com/KDE/breeze" "$TEMP_DIR/breeze"
fi

pushd "$TEMP_DIR/breeze"
do_patch "$SOURCE_DIR/scripts/patches/breeze-cmake.patch"
popd

plasma_build "breeze" "https://github.com/KDE/breeze" \
    -DBUILD_QT5=OFF \
    -DWITH_DECORATIONS=OFF \
    -DWITH_WALLPAPERS=OFF \
    -DCMAKE_DISABLE_FIND_PACKAGE_Qt6Quick=ON

if [[ "$CLEAN_UP" -eq 1 ]]; then
    echo "######### Cleanup"

    rm -rf "$TEMP_DIR"
fi
