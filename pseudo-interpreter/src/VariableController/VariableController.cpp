#include "VariableController.hpp"

variable_controller & variable_controller::instance() {
  static variable_controller _instance;
  return _instance;
}

variable_controller::~variable_controller() {}

math_token& variable_controller::operator[] (std::string variable_name) {
  /*
   * if variable has already been registered -> return it
   * else register new variable and return it
   */
  auto search_result = variables.find(variable_name);
  if (search_result != variables.end()) {
    return search_result->second;
  } else {
    if (detect_function(variable_name) != supported_functions::unindefined) {
      throw std::logic_error("Variable name '" + variable_name + "' has already been used for function");
    }
    std::regex re("[^\\w]");
    if (std::regex_search(variable_name, re)) {
      throw std::logic_error("A variable name can only contain letters and numbers");
    }
    return variables.insert({ variable_name, { "0", token_type::number } }).first->second;
  }
}

size_t variable_controller::read_variable_assigning(std::string str) {
  size_t equals_index    = str.find('='),
         semicolon_index = str.find(';');

  std::string var_name  = str.substr(0, equals_index),
              var_value = str.substr(equals_index + 1, semicolon_index - equals_index - 1);

  this->operator[](var_name) = _execute_expression(
    fill_string_with_variables(var_value)
  );

  return semicolon_index;
}

std::string variable_controller::fill_string_with_variables(std::string str) {
  for (auto it = variables.begin(); it != variables.end(); ++it) {
    std::regex re("\\b" + it->first + "\\b");
    str = std::regex_replace(str, re, it->second.value);
  }
  return str;
}