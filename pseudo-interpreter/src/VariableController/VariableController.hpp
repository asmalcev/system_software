#pragma once

#include <map>
#include <regex>

#include "../../../math_interpreter/src/MathInterpreter/MathInterpreter.hpp"
#include "../../../math_interpreter/src/FunctionInterpreter/FunctionInterpreter.hpp"

class variable_controller {
private:
  variable_controller() {}
  variable_controller(const variable_controller&);
  variable_controller& operator=(variable_controller&);

  std::map<std::string, math_token> variables;

public:
  ~variable_controller();

  static variable_controller & instance();

  math_token & operator[] (std::string variable_name);
  size_t read_variable_assigning(std::string str);
  std::string fill_string_with_variables(std::string str);

};