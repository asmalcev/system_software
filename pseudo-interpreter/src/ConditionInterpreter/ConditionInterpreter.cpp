#include "ConditionInterpreter.hpp"

bool execute_logic_expression(std::string str) {
  math_token result = _execute_expression(
    variable_controller::instance().fill_string_with_variables(str)
  );
  if (result.type == token_type::number) {
    return (bool) std::stoll(result.value);
  }
  if (result.type == token_type::float_number) {
    return (bool) std::stold(result.value);
  }
  return false;
}

std::string find_logic_expression(
  std::string & str,
  std::string operator_name,
  size_t & condition_block_length
) {
  size_t bracket_depth = 0;
  token_type tt;

  if (str[condition_block_length++] != '(') {
    throw std::logic_error(operator_name + " condition must be into rounded brackets and follow behind it");
  } else {
    bracket_depth++;
  }
  while (bracket_depth != 0 && condition_block_length < str.size()) {
    tt = char_type_func(str[condition_block_length++]);
    if (tt == token_type::bracket_open) {
      bracket_depth++;
    } else if (tt == token_type::bracket_close) {
      bracket_depth--;
    }
  }
  if (bracket_depth != 0) {
    throw std::logic_error(operator_name + " condition must be into rounded brackets and follow behind it");
  }
  if (condition_block_length == str.size()) {
    throw std::logic_error(operator_name + " statement missed");
  }

  return str.substr(0, condition_block_length);
}

size_t get_bindex_of_statement(
  std::string & str,
  std::string operator_name,
  size_t & condition_block_length
) {
  size_t bracket_depth = 0;
  token_type tt;

  if (str[condition_block_length++] != '{') {
    throw std::logic_error(operator_name + " statement must be into figured brackets and follow behind condition");
  } else {
    bracket_depth++;
  }
  size_t statement_bindex = condition_block_length;
  while (bracket_depth != 0 && condition_block_length < str.size()) {
    tt = char_type_func(str[condition_block_length++]);
    if (tt == token_type::figured_bracket_open) {
      bracket_depth++;
    } else if (tt == token_type::figured_bracket_close) {
      bracket_depth--;
    }
  }
  if (bracket_depth != 0) {
    throw std::logic_error(operator_name + " statement must be into figured brackets and follow behind condition");
  }

  return statement_bindex;
}

size_t if_proccessing(std::string input) {
  size_t condition_block_length = 0;

  bool condition_state = execute_logic_expression(
    find_logic_expression(input, "Operator-if", condition_block_length)
  );

  size_t statement_bindex = get_bindex_of_statement(input, "Operator-if", condition_block_length);
  size_t statement_eindex = condition_block_length - 1;

  std::regex re("[a-zA-Z]\\w*");
  if (std::sregex_iterator(
        input.begin() + condition_block_length, input.end(), re
      )->str() == "else") {
    condition_block_length += 4;
    size_t tmp = get_bindex_of_statement(input, "Operator-else", condition_block_length);

    if (!condition_state) statement_bindex = tmp;
    if (!condition_state) statement_eindex = condition_block_length;
  }

  interpreter_code(input.substr(statement_bindex, statement_eindex - statement_bindex));
  return condition_block_length - 1;
}