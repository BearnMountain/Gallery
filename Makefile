# Detect OS
ifeq ($(OS),Windows_NT)
    PLATFORM := Windows
    RM := del /Q /F
    RMDIR := rmdir /S /Q
    MKDIR := if not exist
    SEP := \\
    EXE := .exe
    NULL := NUL
else
    UNAME := $(shell uname -s)
    ifeq ($(UNAME),Linux)
        PLATFORM := Linux
    endif
    ifeq ($(UNAME),Darwin)
        PLATFORM := macOS
    endif
    RM := rm -f
    RMDIR := rm -rf
    MKDIR := mkdir -p
    SEP := /
    EXE :=
    NULL := /dev/null
endif

# Project settings
PROJECT := myapp
CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -Ideps/include -Isrc
LDFLAGS := -Ldeps/lib

COMPILE_DB := compile_commands.json

# Platform-specific flags
ifeq ($(PLATFORM),macOS)
    CFLAGS += -DGL_SILENCE_DEPRECATION
endif

# Platform-specific libraries
ifeq ($(PLATFORM),Windows)
    LIBS := -lglfw3 -lopengl32 -lgdi32
endif
ifeq ($(PLATFORM),Linux)
    LIBS := -lglfw3 -lGL -lm -ldl -lpthread -lX11 -lXrandr -lXi -lXxf86vm -lXcursor -lXinerama
endif
ifeq ($(PLATFORM),macOS)
    LIBS := -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif

# Directories
SRC_DIR := src
DEP_DIR := deps
OBJ_DIR := obj
BIN_DIR := bin

# Files
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS_OBJS := $(OBJ_DIR)/glad.o
TARGET := $(BIN_DIR)/$(PROJECT)$(EXE)

# GLFW version
GLFW_VERSION := 3.4
GLFW_ARCHIVE := glfw-$(GLFW_VERSION).zip
GLFW_URL := https://github.com/glfw/glfw/releases/download/$(GLFW_VERSION)/$(GLFW_ARCHIVE)
GLFW_DIR := $(DEP_DIR)/glfw-$(GLFW_VERSION)
GLFW_BUILD := $(GLFW_DIR)/build
GLFW_LIB := $(DEP_DIR)/lib/libglfw3.a

# Dependency files
GLAD_H := $(DEP_DIR)/include/glad/glad.h
GLAD_C := $(DEP_DIR)/src/glad.c
KHR_H := $(DEP_DIR)/include/KHR/khrplatform.h
STB_H := $(DEP_DIR)/include/stb_image.h
GLFW_H := $(DEP_DIR)/include/GLFW/glfw3.h

# Default target
.PHONY: all
all: deps $(TARGET)

# Create directories
$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)

$(BIN_DIR):
	@$(MKDIR) $(BIN_DIR)

$(DEP_DIR)/include/glad:
	@$(MKDIR) $(DEP_DIR)/include/glad

$(DEP_DIR)/include/KHR:
	@$(MKDIR) $(DEP_DIR)/include/KHR

$(DEP_DIR)/include:
	@$(MKDIR) $(DEP_DIR)/include

$(DEP_DIR)/src:
	@$(MKDIR) $(DEP_DIR)/src

$(DEP_DIR)/lib:
	@$(MKDIR) $(DEP_DIR)/lib

# Download GLAD header - using web.archive.org snapshot of working GLAD files
$(GLAD_H): | $(DEP_DIR)/include/glad
	@echo "Downloading GLAD header..."
	@curl -sL -o $(GLAD_H) "https://raw.githubusercontent.com/Dav1dde/glad/refs/heads/c/include/glad/glad.h"
	@echo "#include <glad/glad.h>" > $(DEP_DIR)/include/glad/gl.h

# Download GLAD source
$(GLAD_C): | $(DEP_DIR)/src
	@echo "Downloading GLAD source..."
	@curl -sL -o $(GLAD_C) "https://raw.githubusercontent.com/Dav1dde/glad/refs/heads/c/src/glad.c"

# Download KHR platform header
$(KHR_H): | $(DEP_DIR)/include/KHR
	@echo "Downloading KHR platform header..."
	@curl -sL -o $(KHR_H) https://raw.githubusercontent.com/KhronosGroup/EGL-Registry/main/api/KHR/khrplatform.h

# Download stb_image
$(STB_H): | $(DEP_DIR)/include
	@echo "Downloading stb_image..."
	@curl -sL -o $(STB_H) https://raw.githubusercontent.com/nothings/stb/master/stb_image.h

# Download and build GLFW
$(DEP_DIR)/$(GLFW_ARCHIVE): | $(DEP_DIR)
	@echo "Downloading GLFW $(GLFW_VERSION)..."
	@curl -sL -o $(DEP_DIR)/$(GLFW_ARCHIVE) $(GLFW_URL)

$(GLFW_DIR): $(DEP_DIR)/$(GLFW_ARCHIVE)
	@echo "Extracting GLFW..."
	@cd $(DEP_DIR) && unzip -q $(GLFW_ARCHIVE)

$(GLFW_LIB): $(GLFW_DIR) | $(DEP_DIR)/lib
	@echo "Building GLFW (this may take a minute)..."
	@$(MKDIR) $(GLFW_BUILD)
	@cd $(GLFW_BUILD) && cmake -DCMAKE_INSTALL_PREFIX=../../ -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF .. > $(NULL) 2>&1
	@cd $(GLFW_BUILD) && cmake --build . --config Release > $(NULL) 2>&1
	@cd $(GLFW_BUILD) && cmake --install . > $(NULL) 2>&1
	@echo "GLFW built successfully!"

$(GLFW_H): $(GLFW_LIB)

# Ensure dependencies are downloaded
.PHONY: deps
deps: $(GLAD_H) $(GLAD_C) $(KHR_H) $(STB_H) $(GLFW_H)
	@echo "All dependencies ready!"

# Compile GLAD
$(OBJ_DIR)/glad.o: $(GLAD_C) $(GLAD_H) $(KHR_H) | $(OBJ_DIR)
	@echo "Compiling GLAD..."
	@$(CC) $(CFLAGS) -c $(GLAD_C) -o $@

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) deps
	@echo "Compiling $<..."
	@$(MKDIR) $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Link executable
$(TARGET): $(DEPS_OBJS) $(OBJS) | $(BIN_DIR)
	@echo "Linking $(PROJECT)..."
	@$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)
	@echo "Build complete: $(TARGET)"

# Compiler Commands for NVIM
.PHONY: compdb
compdb:
	@echo "Generating compile_commands.json..."
	@bear -- $(MAKE) --no-print-directory
	@echo "compile_commands.json generated"


# Run the program
.PHONY: run
run: $(TARGET)
	@./$(TARGET)
	-@$(RMDIR) $(OBJ_DIR) 2>$(NULL) || true

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	-@$(RMDIR) $(BIN_DIR) 2>$(NULL) || true
	@echo "Clean complete!"

# Clean everything including dependencies
.PHONY: cleanall
cleanall: clean
	@echo "Removing dependencies..."
	-@$(RMDIR) $(DEP_DIR) 2>$(NULL) || true
	@echo "Full clean complete!"
