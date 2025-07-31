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
    echo "    -v, --version           Qt version (default: $QT_VERSION)"
    echo "    -n, --no-clean          keep working files"
}

while [[ $# -gt 0 ]]; do
    case $1 in
        -p|--prefix)
            PREFIX="$(readlink -f "$2")"
            shift
            shift
            ;;
        -v|--version)
            QT_VERSION="$2"
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

echo "##### Install Qt $QT_VERSION"

case "$PLATFORM" in
    linux)
        "$PYTHON" -m pip install --ignore-installed -q aqtinstall
        "$PYTHON" -m aqt install-qt -O "$TEMP_DIR/qt" linux desktop "$QT_VERSION" linux_gcc_64
        QT_DIR="$TEMP_DIR/qt/$QT_VERSION/gcc_64"
        ;;
    windows)
        curl -L "https://github.com/miurahr/aqtinstall/releases/download/v3.3.0/aqt_x64.exe" -o "$DOWNLOAD_DIR/aqt.exe"
        "$DOWNLOAD_DIR/aqt.exe" install-qt -O "$TEMP_DIR/qt" windows desktop "$QT_VERSION" win64_llvm_mingw
        QT_DIR="$TEMP_DIR/qt/$QT_VERSION/llvm-mingw_64"
        ;;
esac

cp -r "$QT_DIR"/. "$PREFIX"

cleanup
