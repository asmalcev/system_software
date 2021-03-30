GCC = g++

BUILD_DIR 	= ./build
BUILD_FLAGS =
BINNAME     = pseudo_interpreter

MAIN_PATH					= ./src/main.cpp
TEST_PATH					= ./src/test.cpp
LOOP_IN_DIR			  = ./src/LoopInterpreter
PSEUDO_IN_DIR 	  = ./src/PseudoInterpreter
VARIABLE_C_DIR		= ./src/VariableController
CONDITION_IN_DIR 	= ./src/ConditionInterpreter
INTERPRETER_DIR 	= ../math_interpreter/src/MathInterpreter
FUNCTION_IN_DIR 	= ../math_interpreter/src/FunctionInterpreter

build: $(BUILD_DIR)/main.o $(BUILD_DIR)/interpreter.o $(BUILD_DIR)/func_interpreter.o $(BUILD_DIR)/pseudo_interpreter.o $(BUILD_DIR)/condition_interpreter.o $(BUILD_DIR)/variable_controller.o $(BUILD_DIR)/loop_interpreter.o
	g++ $^ $(BUILD_FLAGS) -o $(BUILD_DIR)/$(BINNAME)

build_test: $(BUILD_DIR)/test.o $(BUILD_DIR)/interpreter.o $(BUILD_DIR)/func_interpreter.o $(BUILD_DIR)/pseudo_interpreter.o $(BUILD_DIR)/condition_interpreter.o $(BUILD_DIR)/variable_controller.o $(BUILD_DIR)/loop_interpreter.o
	g++ $^ $(BUILD_FLAGS) -o $(BUILD_DIR)/test_$(BINNAME)

$(BUILD_DIR)/main.o: $(MAIN_PATH)
	g++ $(BUILD_FLAGS) -c $< -o $@

$(BUILD_DIR)/test.o: $(TEST_PATH)
	g++ $(BUILD_FLAGS) -c $< -o $@

$(BUILD_DIR)/interpreter.o: $(INTERPRETER_DIR)/*.cpp
	g++ $(BUILD_FLAGS) -c $< -o $@

$(BUILD_DIR)/func_interpreter.o: $(FUNCTION_IN_DIR)/*.cpp
	g++ $(BUILD_FLAGS) -c $< -o $@

$(BUILD_DIR)/pseudo_interpreter.o: $(PSEUDO_IN_DIR)/*.cpp
	g++ $(BUILD_FLAGS) -c $< -o $@

$(BUILD_DIR)/condition_interpreter.o: $(CONDITION_IN_DIR)/*.cpp
	g++ $(BUILD_FLAGS) -c $< -o $@

$(BUILD_DIR)/variable_controller.o: $(VARIABLE_C_DIR)/*.cpp
	g++ $(BUILD_FLAGS) -c $< -o $@

$(BUILD_DIR)/loop_interpreter.o: $(LOOP_IN_DIR)/*.cpp
	g++ $(BUILD_FLAGS) -c $< -o $@

run: $(BUILD_DIR)/$(BINNAME)
	$(BUILD_DIR)/$(BINNAME) $(ARGS)

run_test: $(BUILD_DIR)/test_$(BINNAME)
	$(BUILD_DIR)/test_$(BINNAME)

clear:
	rm -rf $(BUILD_DIR)/*

all: build

.PHONY: all clear build run build_test run_test