GCC = g++

BUILD_DIR 	= ./build
BUILD_FLAGS =
BINNAME     = math_interpreter

MAIN_PATH					= ./src/main.cpp
TEST_PATH					= ./src/test.cpp
INTERPRETER_DIR 	= ./src/MathInterpreter
FUNCTION_IN_DIR 	= ./src/FunctionInterpreter

build: $(BUILD_DIR)/main.o $(BUILD_DIR)/interpreter.o $(BUILD_DIR)/func_interpreter.o
	g++ $^ $(BUILD_FLAGS) -o $(BUILD_DIR)/$(BINNAME)

build_test: $(BUILD_DIR)/test.o $(BUILD_DIR)/interpreter.o $(BUILD_DIR)/func_interpreter.o
	g++ $^ $(BUILD_FLAGS) -o $(BUILD_DIR)/test_$(BINNAME)

$(BUILD_DIR)/main.o: $(MAIN_PATH)
	g++ $(BUILD_FLAGS) -c $< -o $@

$(BUILD_DIR)/test.o: $(TEST_PATH)
	g++ $(BUILD_FLAGS) -c $< -o $@

$(BUILD_DIR)/interpreter.o: $(INTERPRETER_DIR)/*.cpp
	g++ $(BUILD_FLAGS) -c $< -o $@

$(BUILD_DIR)/func_interpreter.o: $(FUNCTION_IN_DIR)/*.cpp
	g++ $(BUILD_FLAGS) -c $< -o $@

run: $(BUILD_DIR)/$(BINNAME)
	$(BUILD_DIR)/$(BINNAME) $(ARGS)

run_test: $(BUILD_DIR)/test_$(BINNAME)
	$(BUILD_DIR)/test_$(BINNAME)

clear:
	rm -rf $(BUILD_DIR)/*

all: build

.PHONY: all clear build run build_test run_test