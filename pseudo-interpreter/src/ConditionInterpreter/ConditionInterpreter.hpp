#pragma once

#include <iostream>

#include "../PseudoInterpreter/PseudoInterpreter.hpp"
#include "../VariableController/VariableController.hpp"

size_t if_proccessing(std::string input);

bool execute_logic_expression(std::string str);

std::string find_logic_expression(
  std::string & str,
  std::string operator_name,
  size_t & length
);
size_t get_bindex_of_statement(
  std::string & str,
  std::string operator_name,
  size_t & condition_block_length
);