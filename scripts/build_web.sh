#!/usr/bin/env bash
set -euo pipefail

EMSDK_DIR="/home/ritabrata/Documents/emsdk"

if [[ ! -f "$EMSDK_DIR/emsdk_env.sh" ]]; then
  echo "emsdk_env.sh not found at: $EMSDK_DIR" >&2
  exit 1
fi

# shellcheck disable=SC1090
source "$EMSDK_DIR/emsdk_env.sh"

export EM_CACHE="$(pwd)/temp/emcache"
mkdir -p "$EM_CACHE"

mkdir -p build-web
emcmake cmake -S . -B build-web -DCMAKE_BUILD_TYPE=Release
cmake --build build-web --parallel
