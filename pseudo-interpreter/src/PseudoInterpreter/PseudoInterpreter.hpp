#pragma once

#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>

#include "../../../math_interpreter/src/MathInterpreter/MathInterpreter.hpp"

#include "../LoopInterpreter/LoopInterpreter.hpp"
#include "../VariableController/VariableController.hpp"
#include "../ConditionInterpreter/ConditionInterpreter.hpp"

std::string clear_input(std::ifstream& input);
std::string clear_input(std::string input);
void interpreter_code(std::string input);
