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

cmake -S "$PROJECT_DIR" -B "$PROJECT_DIR/build" -G Ninja \
    -DCMAKE_BUILD_TYPE="Release" \
    -DCMAKE_PREFIX_PATH="$PREFIX" \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR"

cmake --build "$PROJECT_DIR/build" --parallel
