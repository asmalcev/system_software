GCC = g++

BUILD_DIR 	= ./build
BUILD_FLAGS =
BINNAME     = math_interpreter

MAIN_PATH 	= ./src/main.cpp

build: $(BUILD_DIR)/main.o
	g++ $^ $(BUILD_FLAGS) -o $(BUILD_DIR)/$(BINNAME)

$(BUILD_DIR)/main.o: $(MAIN_PATH)
	g++ $(BUILD_FLAGS) -c $< -o $@

run: $(BUILD_DIR)/$(BINNAME)
	$(BUILD_DIR)/$(BINNAME)

clear:
	rm -rf $(BUILD_DIR)/*

all: build

.PHONY: all clear build run