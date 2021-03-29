#include "ConditionInterpreter.hpp"

static bool check_logic_expression(std::string str) {
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
  if (bracket_depth != 0) {
    throw std::logic_error("Operator-if condition must be into rounded brackets and follow behind it");
  }
  if (condition_block_length == input.size()) {
    throw std::logic_error("Operator-if statement missed");
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
      throw std::logic_error("Operator-if else-statement must be into figured brackets and follow behind condition");
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
      throw std::logic_error("Operator-if else-statement must be into figured brackets and follow behind condition");
    }

    if (!condition_state) statement_eindex = condition_block_length;
  }

  interpreter_code(input.substr(statement_bindex, statement_eindex - statement_bindex));
  return condition_block_length - 1;
}