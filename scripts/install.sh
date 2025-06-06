#!/bin/bash
set -e

echo "[*] Building openai-c with CMake..."

ROOT_DIR="$(dirname "$0")/.."

cd "$ROOT_DIR"

mkdir -p build
cd build
cmake ..
make
sudo make install

echo "[✓] Installed openai-c!"
