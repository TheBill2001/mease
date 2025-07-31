#!/bin/bash

set -e

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
SCRIPT_DIR="$(readlink -f "$SCRIPT_DIR")"

PROJECT_DIR="$(readlink -f "$SCRIPT_DIR/../..")"

PREFIX="$PROJECT_DIR/prefix"
INSTALL_DIR="$PROJECT_DIR/install"

QT_VERSION=6.9.1
KF_VERSION=6.16.0
PLASMA_VERSION=6.4.3
PLASMA_WAYLAND_PROTOCOLS_VERSION=1.18.0

KEEP=0

case "$(uname -s)" in
    Linux*)
        PLATFORM=linux
        ;;
    MINGW64_NT*|MSYS_NT*)
        PLATFORM=windows
        ;;
    *)
        echo "UNSUPPORTED: $(uname -s)"
        exit 1
        ;;
esac

function download() {
    local _temp="$DOWNLOAD_DIR/.$2.tmp"

    curl -L -o "$_temp" "$1" && mv "$_temp" "$DOWNLOAD_DIR/$2"

    if [[ -e "$_temp" ]]; then
        rm "$_temp"
    fi
}

function make_folders() {
    if ! [[ -d "$DOWNLOAD_DIR" ]]; then
        mkdir -p "$DOWNLOAD_DIR"
    fi

    if ! [[ -d "$SRC_DIR" ]]; then
        mkdir -p "$SRC_DIR"
    fi
}

function set_folders() {
    TEMP_DIR="$(dirname -- "$PREFIX")/prefix-tmp"
    DOWNLOAD_DIR="$TEMP_DIR/download"
    SRC_DIR="$TEMP_DIR/src"
}

function set_python() {
    if ! [[ $(type -P python3) ]]; then
        echo "Python 3 is not installed"
        exit 1
    fi

    echo "##### Setting up Python environment"

    if ! [[ -d "$TEMP_DIR/venv" ]]; then
        python3 -m venv "$TEMP_DIR/venv"
    fi

    source "$TEMP_DIR/venv/bin/activate"
    PYTHON="$(type -P python3)"
    deactivate

    PYTHON_PREFIX="$("$PYTHON" -c "import sys; print(sys.prefix)")"

    echo "-- Python executable: '$PYTHON'"
    echo "-- Python install dir: '$PYTHON_PREFIX'"
}

function cleanup() {
    if ! [[ "$KEEP" -eq 0 ]]; then
        echo "##### Clean up"
        rm -rf "$TEMP_DIR"
    fi
}
