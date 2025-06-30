#!/bin/bash
set -e
BUILD_DIR="build"

# Colors
RED=31
GREEN=32
YELLOW=33
BLUE=34
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
say "~" $YELLOW "Checking for build directory..."

[ -d "$BUILD_DIR" ] && {
	say "!" $RED "Deleting existing build directory..."
	rm -rf "$BUILD_DIR"
} || say "+" $GREEN "No build directory found..."

say "+" $GREEN "Creating build directory..."
echo
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Detect build system
if command -v ninja >/dev/null 2>&1; then
	GENERATOR="Ninja"
	BUILD_CMD="ninja"
else
	GENERATOR="Unix Makefiles"
	BUILD_CMD="make -j$(nproc)"
fi

say "+" $GREEN "Running CMake with $CC_NAME and $GENERATOR..."

CC="$CC" cmake -G "$GENERATOR" \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
	..

echo
say "*" $BLUE "Building project with $GENERATOR..."
if $BUILD_CMD; then
	echo
	say "!" $GREEN "Build complete."
else
	echo
	say "!" $RED "Build failed."
	exit 1
fi
