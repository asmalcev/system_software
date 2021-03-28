#include <iostream>
#include <regex>
#include <sstream>
#include <deque>
#include <fstream>

#include "../../math_interpreter/src/MathInterpreter/MathInterpreter.hpp"

void interpreter_code(std::string input);

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

bool check_logic_expression(std::string str) {
  math_token result = _execute_expression(str);
  if (result.type == token_type::number) {
    return (bool) std::stoll(result.value);
  }
  if (result.type == token_type::float_number) {
    return (bool) std::stold(result.value);
  }
  return false;
}

size_t if_proccessing(std::string input) {
  size_t bracket_depth    = 0,
         condition_block_length = 0;
  token_type tt;

  if (input[condition_block_length++] != '(') {
    throw std::logic_error("Operator-if condition must be into rounded brackets and follow behind it");
  } else {
    bracket_depth++;
  }
  while (bracket_depth != 0 && condition_block_length < input.size()) {
    tt = char_type_func(input[condition_block_length++]);
    if (tt == token_type::bracket_open) {
      bracket_depth++;
    } else if (tt == token_type::bracket_close) {
      bracket_depth--;
    }
  }
  if (condition_block_length >= input.size()) {
    throw std::logic_error("Operator-if condition must be into rounded brackets and follow behind it");
  }

  bool condition_state = check_logic_expression(input.substr(0, condition_block_length));

  if (input[condition_block_length++] != '{') {
    throw std::logic_error("Operator-if statement must be into figured brackets and follow behind condition");
  } else {
    bracket_depth++;
  }
  size_t statement_bindex = condition_block_length;
  while (bracket_depth != 0 && condition_block_length < input.size()) {
    tt = char_type_func(input[condition_block_length++]);
    if (tt == token_type::figured_bracket_open) {
      bracket_depth++;
    } else if (tt == token_type::figured_bracket_close) {
      bracket_depth--;
    }
  }
  if (bracket_depth != 0) {
    throw std::logic_error("Operator-if statement must be into figured brackets and follow behind condition");
  }
  size_t statement_eindex = condition_block_length - 1;

  std::regex re("[a-zA-Z]\\w*");
  if (std::sregex_iterator(
        input.begin() + condition_block_length, input.end(), re
      )->str() == "else") {

    condition_block_length += 4;
    if (input[condition_block_length++] != '{') {
      throw std::logic_error("Operator-if statement must be into figured brackets and follow behind condition");
    } else {
      bracket_depth++;
    }

    if (!condition_state) statement_bindex = condition_block_length;

    while (bracket_depth != 0 && condition_block_length < input.size()) {
      tt = char_type_func(input[condition_block_length++]);
      if (tt == token_type::figured_bracket_open) {
        bracket_depth++;
      } else if (tt == token_type::figured_bracket_close) {
        bracket_depth--;
      }
    }
    if (char_type_func(input[condition_block_length - 1]) != token_type::figured_bracket_close) {
      throw std::logic_error("Operator-if statement must be into figured brackets and follow behind condition");
    }

    if (!condition_state) statement_eindex = condition_block_length;
  }

  interpreter_code(input.substr(statement_bindex, statement_eindex - statement_bindex));
  return condition_block_length - 1;
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

int main(int argc, char ** argv) {
  if (argc < 2) {
    std::cout << "Not enough arguments" << std::endl;
    return 0;
  }

  std::ifstream input(argv[1]);
  std::string line;

  if (!input.is_open()) {
    std::cout << "Can't open " << argv[1] << " or it doesn't exist" << std::endl;
    return 0;
  }

  interpreter_code(clear_input(input));
}