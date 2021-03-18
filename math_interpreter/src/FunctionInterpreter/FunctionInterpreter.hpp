#pragma once

#include <iostream>
#include <string>
#include <list>

#include "../MathInterpreter/MathInterpreter.hpp"

enum supported_functions {
  f_sin,
  unindefined
};

typedef std::string argument;

struct math_token execute_function(std::string, size_t & p_length);