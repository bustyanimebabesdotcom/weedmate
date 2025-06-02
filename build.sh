#!/bin/bash
set -e

BUILD_DIR="build"

# Init
echo -e "[\e[33m*\e[0m] Checking for build directory..."

if [ -d "$BUILD_DIR" ]; then
    echo -e "[\e[31m!\e[0m] Deleting existing build directory..."
    rm -rf "$BUILD_DIR"

else
    echo -e "[\e[32m+\e[0m] No build directory found..."
fi

echo -e "[\e[32m+\e[0m] Creating build directory..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Select build system
echo -e "\n[\e[33m?\e[0m] Select your build system:\n"
echo -e "    \e[32m1\e[0m. Ninja"
echo -e "    \e[32m2\e[0m. Make"
echo
read -rp "> " choice

case "$choice" in
	1)
		GENERATOR="Ninja"
		BUILD_CMD="ninja"
		;;
	2)
		GENERATOR="Unix Makefiles"
		BUILD_CMD="make -j$(nproc)"
		;;
	*)
		echo -e "[\e[31m!\e[0m] Invalid choice. Defaulting to Ninja."
		GENERATOR="Ninja"
		BUILD_CMD="ninja"
		;;
esac

# Clean up build system prompt
for _ in {1..6}; do tput cuu1; tput el; done

# Run cmake
echo -e "[\e[32m+\e[0m] Running CMake with Clang and $GENERATOR..."
CC=clang cmake -G "$GENERATOR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    ..

echo -e "\n[\e[33m?\e[0m] Build now? [y/N]\n"
read -rp "> " BUILD_NOW

# Clean up build prompt
for _ in {1..3}; do tput cuu1; tput el; done

# Finish
if [[ "$BUILD_NOW" =~ ^[Yy]$ ]]; then
    echo -e "[\e[32m+\e[0m] Building project with $GENERATOR..."
    if $BUILD_CMD; then
    	echo -e "\n[\e[32m!\e[0m] Build complete."
	else
		echo -e "\n[\e[31m!\e[0m] Build failed."
		exit 1
	fi
else
    echo -e "[\e[33m~\e[0m] Build skipped. Run '$BUILD_CMD' later in ./build"
fi
