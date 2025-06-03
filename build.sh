#!/bin/bash
set -e
BUILD_DIR="build"

# Colors
RED=31
GREEN=32
YELLOW=33
INDENT="    "

say() {
	local symbol="$1"
	local color="$2"
	local msg="$3"
	local indent="${4:-}"
	echo -e "${indent}[\e[${color}m${symbol}\e[0m] $msg"
}

# Detect architecture and choose compiler
UNAME_ARCH="$(uname -m)"
case "$UNAME_ARCH" in
	aarch64|arm64|armv7l|armv6l)
		CC="gcc"
		CC_NAME="GCC"
		;;
	*)
		CC="clang"
		CC_NAME="Clang"
		;;
esac

# Init
say "*" $YELLOW "Checking for build directory..."

[ -d "$BUILD_DIR" ] && {
	say "!" $RED "Deleting existing build directory..."
    rm -rf "$BUILD_DIR"
} || say "+" $GREEN "No build directory found..."

say "+" $GREEN "Creating build directory..."
echo
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Select build system
say "?" $YELLOW "Select your build system:"
echo
say "1" $GREEN "Ninja" "$INDENT"
say "2" $GREEN "Make" "$INDENT"
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
		GENERATOR="Ninja"
		BUILD_CMD="ninja"
		;;
esac

# Clean up build system prompt
for _ in {1..6}; do tput cuu1; tput el; done

# Run cmake
say "+" $GREEN "Running CMake with $CC_NAME and $GENERATOR..."
CC="$CC" cmake -G "$GENERATOR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    ..

# Build prompt
echo
say "?" $YELLOW "Build now? [y/N]"
echo
read -rp "> " BUILD_NOW
: "${BUILD_NOW:=n}"

# Clean up build prompt
for _ in {1..3}; do tput cuu1; tput el; done

# Finish
[[ "$BUILD_NOW" =~ ^[Yy]$ ]] && {
	say "+" $GREEN "Building project with $GENERATOR..."
    if $BUILD_CMD; then
		echo
		say "!" $GREEN "Build complete."
	else
		echo
		say "!" $RED "Build failed."
		exit 1
	fi
} || say "~" $YELLOW "Build skipped. Run '$BUILD_CMD' later in ./build"
