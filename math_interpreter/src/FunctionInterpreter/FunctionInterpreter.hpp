#pragma once

#include <iostream>
#include <string>
#include <list>

#include "../MathInterpreter/MathInterpreter.hpp"

enum supported_functions {
  f_sin,
  f_cos,
  f_tan,
  f_asin,
  f_acos,
  f_atan,
  f_println,
  unindefined
};

std::string execute_function(std::string str);
struct math_token _execute_function(std::string str, size_t & p_length);