# Detect OS
ifeq ($(OS),Windows_NT)
    PLATFORM := Windows
    RM := del /Q /F
    RMDIR := rmdir /S /Q
    MKDIR := mkdir
    SEP := \\
    EXE := .exe
    CURL := curl -L -o
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
    CURL := curl -L -o
endif

# Project settings
PROJECT := myapp
CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -Ideps/include -Isrc
LDFLAGS := -Ldeps/lib

# Platform-specific libraries
ifeq ($(PLATFORM),Windows)
    LIBS := -lopengl32 -lgdi32
endif
ifeq ($(PLATFORM),Linux)
    LIBS := -lGL -lm -ldl -lpthread
endif
ifeq ($(PLATFORM),macOS)
    LIBS := -framework OpenGL -framework Cocoa -framework IOKit
endif

# Directories
SRC_DIR := src
DEP_DIR := deps
OBJ_DIR := obj
BIN_DIR := bin

# Files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS_OBJS := $(OBJ_DIR)/glad.o
TARGET := $(BIN_DIR)/$(PROJECT)$(EXE)

# GLAD and stb_image URLs
GLAD_URL := https://raw.githubusercontent.com/Dav1dde/glad/glad2/glad/glad.h
GLAD_C_URL := https://raw.githubusercontent.com/Dav1dde/glad/glad2/glad/gl.h
STB_IMAGE_URL := https://raw.githubusercontent.com/nothings/stb/master/stb_image.h

# Default target
.PHONY: all
all: deps $(TARGET)

# Create directories
$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)

$(BIN_DIR):
	@$(MKDIR) $(BIN_DIR)

$(DEP_DIR)/include:
	@$(MKDIR) $(DEP_DIR)$(SEP)include$(SEP)glad
	@$(MKDIR) $(DEP_DIR)$(SEP)include$(SEP)KHR

$(DEP_DIR)/src:
	@$(MKDIR) $(DEP_DIR)$(SEP)src

# Download dependencies
.PHONY: deps
deps: $(DEP_DIR)/include $(DEP_DIR)/src
	@echo "Downloading dependencies..."
	@if not exist "$(DEP_DIR)$(SEP)include$(SEP)glad$(SEP)glad.h" $(CURL) $(DEP_DIR)$(SEP)include$(SEP)glad$(SEP)glad.h $(GLAD_URL) 2>nul || :
	@if not exist "$(DEP_DIR)$(SEP)include$(SEP)KHR$(SEP)khrplatform.h" $(CURL) $(DEP_DIR)$(SEP)include$(SEP)KHR$(SEP)khrplatform.h https://raw.githubusercontent.com/KhronosGroup/EGL-Registry/main/api/KHR/khrplatform.h 2>nul || :
	@if not exist "$(DEP_DIR)$(SEP)src$(SEP)glad.c" $(CURL) $(DEP_DIR)$(SEP)src$(SEP)glad.c https://raw.githubusercontent.com/Dav1dde/glad/glad2/src/gl.c 2>nul || :
	@if not exist "$(DEP_DIR)$(SEP)include$(SEP)stb_image.h" $(CURL) $(DEP_DIR)$(SEP)include$(SEP)stb_image.h $(STB_IMAGE_URL) 2>nul || :
	@echo "Dependencies ready!"

# Compile GLAD
$(OBJ_DIR)/glad.o: $(DEP_DIR)/src/glad.c | $(OBJ_DIR)
	@echo "Compiling GLAD..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Link executable
$(TARGET): $(DEPS_OBJS) $(OBJS) | $(BIN_DIR)
	@echo "Linking $(PROJECT)..."
	@$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)
	@echo "Build complete: $(TARGET)"

# Run the program
.PHONY: run
run: $(TARGET)
	@./$(TARGET)

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	@$(RMDIR) $(OBJ_DIR) 2>nul || $(RM) $(OBJ_DIR)$(SEP)*.o 2>nul || :
	@$(RMDIR) $(BIN_DIR) 2>nul || $(RM) $(TARGET) 2>nul || :
	@echo "Clean complete!"

# Clean everything including dependencies
.PHONY: cleanall
cleanall: clean
	@echo "Removing dependencies..."
	@$(RMDIR) $(DEP_DIR) 2>nul || :
	@echo "Full clean complete!"

# Help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  make          - Build the project"
	@echo "  make deps     - Download dependencies"
	@echo "  make run      - Build and run the project"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make cleanall - Remove build artifacts and dependencies"
	@echo ""
	@echo "Platform detected: $(PLATFORM)"
