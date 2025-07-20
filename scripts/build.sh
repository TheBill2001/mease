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
        -d|--debug)
            DEBUG=1
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

CC="$(type -P clang)"
export CC

CXX="$(type -P clang++)"
export CXX

BUILD_TYPE="Release"
if [[ "$DEBUG" -eq 1 ]]; then
    BUILD_TYPE="Debug"
fi

cmake -S "$SOURCE_DIR" -B "$SOURCE_DIR/build" -G Ninja \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_PREFIX_PATH="$QT_DIR" \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR"

cmake --build "$SOURCE_DIR/build" --parallel
