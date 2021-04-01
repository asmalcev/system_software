#include "LoopInterpreter.hpp"

#include <utility>

size_t while_proccessing(std::string input) {
  size_t condition_block_length = 0;

  std::string condition_string = find_logic_expression(input, "Operator-while", condition_block_length);
  size_t statement_bindex      = get_bindex_of_statement(input, "Operator-while", condition_block_length);
  size_t statement_eindex      = condition_block_length - 1;

  while (execute_logic_expression(condition_string)) {
    interpreter_code(input.substr(statement_bindex, statement_eindex - statement_bindex));
  }
  return condition_block_length - 1;
}

static std::pair<std::string, std::string>
get_condition_from_for(std::string & str, size_t & condition_block_length) {
  size_t bracket_depth = 0,
         tmp_index     = 0;
  token_type tt;
  

  if (str[condition_block_length++] != '(') {
    throw std::logic_error("Operator-for condition must be into rounded brackets and follow behind it");
  } else {
    bracket_depth++;
  }

  // interpreter first part of statement in rounded brackets
  tmp_index = str.find(';') + 1;
  interpreter_code(str.substr(condition_block_length, tmp_index - condition_block_length));
  condition_block_length = tmp_index;

  tmp_index = str.find(';', condition_block_length);
  std::string condition_string = str.substr(condition_block_length, tmp_index - condition_block_length);
  condition_block_length = ++tmp_index;

  while (bracket_depth != 0 && condition_block_length < str.size()) {
    tt = char_type_func(str[condition_block_length++]);
    if (tt == token_type::bracket_open) {
      bracket_depth++;
    } else if (tt == token_type::bracket_close) {
      bracket_depth--;
    }
  }
  if (bracket_depth != 0) {
    throw std::logic_error("Operator-for condition must be into rounded brackets and follow behind it");
  }
  if (condition_block_length == str.size()) {
    throw std::logic_error("Operator-for statement missed");
  }
  std::string after_step_statement = str.substr(tmp_index, condition_block_length - tmp_index - 1) + ';';

  return std::pair<std::string, std::string>(condition_string, after_step_statement);
}

size_t for_proccessing(std::string input) {
  size_t condition_block_length = 0;

  auto condition_pair     = get_condition_from_for(input, condition_block_length);
  size_t statement_bindex = get_bindex_of_statement(input, "Operator-for", condition_block_length);
  size_t statement_eindex = condition_block_length - 1;

  // std::cout << condition_pair.first << std::endl;
  while (execute_logic_expression(condition_pair.first)) {
    // execute step statement
    interpreter_code(input.substr(statement_bindex, statement_eindex - statement_bindex));
    // execute after-step statement
    interpreter_code(condition_pair.second);
  }
  return condition_block_length - 1;
}