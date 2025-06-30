.PHONY: all configure build install clean run script help debug

# Defaults
PROJECT_NAME		?= weedmate
BUILD_DIR			?= build
BUILD_COMMAND		?= ./$(PROJECT_NAME)
PREFIX				?= /usr/local
CMAKE_BUILD_TYPE	?= Release
CMAKE_GENERATOR		?= Ninja

# Default to clang; gcc on ARM
UNAME_ARCH := $(shell uname -m)
ifeq ($(filter arm aarch64 arm64 armv7l armv6l,$(UNAME_ARCH)),)
	C_COMPILER		?= clang
	C_COMPILER_NAME := Clang
else
	C_COMPILER		?= gcc
	C_COMPILER_NAME := GCC
endif

# Detect generator if Ninja is missing
ifeq ($(shell command -v ninja 2>/dev/null),)
	ifeq ($(CMAKE_GENERATOR),Ninja)
		CMAKE_GENERATOR := Unix Makefiles
	endif
endif

# Check that build.sh exists
SCRIPT_EXISTS := $(wildcard build.sh)

# Check dependencies
CMAKE := $(shell command -v cmake || (echo "Error: cmake not found" >&2; exit 1)) 

# Targets
ifneq ($(wildcard $(BUILD_DIR)/.configured),)
all: build
else
ifneq ($(SCRIPT_EXISTS),)
all: script
else
all:
	@echo "[!] build.sh not found, using direct configuration..."
	@$(MAKE) --no-print-directory configure build
endif
endif

script:
	@bash ./build.sh
	@
	@touch $(BUILD_DIR)/.configured

configure: $(BUILD_DIR)/.configured

build: $(BUILD_DIR)/.configured
	@cmake --build $(BUILD_DIR) -j$(shell nproc)

$(BUILD_DIR)/.configured:
	@mkdir -p $(BUILD_DIR)
	@echo "[+] Running CMake with $(C_COMPILER_NAME) and $(CMAKE_GENERATOR)…"
	@cmake -B $(BUILD_DIR) -G "$(CMAKE_GENERATOR)" \
		-DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) \
		-DCMAKE_C_COMPILER=$(C_COMPILER) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	@echo
	@touch $(BUILD_DIR)/.configured

debug: CMAKE_BUILD_TYPE := Debug
debug: clean build

install: build
	@cmake --install $(BUILD_DIR) --prefix $(DESTDIR)$(PREFIX)

clean:
	@echo "[!] Cleaning up..."
	@rm -rf $(BUILD_DIR)

run: build
	@cd $(BUILD_DIR) && $(BUILD_COMMAND)

help:
	@echo "Usage: make [OPTION]"
	@echo
	@echo "Targets:"
	@echo "  all            - Build the project (default)."
ifneq ($(SCRIPT_EXISTS),)
	@echo "  script         - Run the build script (detected)."
endif
	@echo "  configure      - Run CMake configuration." 
	@echo "  build          - Non-interactive configuration and build."
	@echo "  debug          - Deletes folder and makes debug build."
	@echo "  run            - Run the built binary."
	@echo "  install        - Install the project."
	@echo "  clean          - Remove build directory."
	@echo "  help           - Show this help message."
