#include "LoopInterpreter.hpp"

size_t while_proccessing(std::string input) {
  size_t condition_block_length = 0;
  token_type tt;

  std::string condition_string = find_logic_expression(input, "Operator-while", condition_block_length);
  size_t statement_bindex      = get_bindex_of_statement(input, "Operator-while", condition_block_length);
  size_t statement_eindex      = condition_block_length - 1;

  while (execute_logic_expression(condition_string)) {
    interpreter_code(input.substr(statement_bindex, statement_eindex - statement_bindex));
  }
  return condition_block_length - 1;
}