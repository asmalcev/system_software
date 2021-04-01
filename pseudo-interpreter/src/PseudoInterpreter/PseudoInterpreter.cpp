#include "PseudoInterpreter.hpp"

std::string clear_input(std::ifstream& input) {
  std::stringstream ss;
  char c;
  while (input.get(c)) {
    if (!std::isspace(c)) {
      ss << c;
    }
  }
  return ss.str();
}

std::string clear_input(std::string input) {
  std::stringstream ss;
  for (auto c : input) {
    if (!std::isspace(c)) {
      ss << c;
    }
  }
  return ss.str();
}

void interpreter_code(std::string input) {
  // regex for word starting with letter
  std::regex re("[a-zA-Z]\\w*");
  std::sregex_iterator it;
  std::string buffer;
  size_t length_buffer = 0;

  for (size_t i = 0; i < input.size(); ++i) {
    if (char_type_math(input[i]) == token_type::symbol) {
      it = std::sregex_iterator(input.begin() + i, input.end(), re);
      buffer = it->str();

      if (buffer == "if") {
        i += buffer.size() + if_proccessing(input.substr(i + 2));
      } else if (buffer == "while") {
        i += buffer.size() + while_proccessing(input.substr(i + 5));
        
      } else if (buffer == "for") {
        i += buffer.size() + for_proccessing(input.substr(i + 3));

      } else {

        if (input[i + buffer.size()] == '=') {
          // variable assigning
          i += variable_controller::instance().read_variable_assigning(input.substr(i));

        } else {
          _execute_function(input.substr(i), length_buffer);
          i += length_buffer;
          length_buffer = 0;

          if (input[i] != ';') {
            throw std::logic_error("Missed ; symbol");
          }
        }
      }
    }
  }
}