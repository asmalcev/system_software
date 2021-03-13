#include <iostream>
#include <string>

#include "MathInterpreter/MathInterpreter.hpp"

int main() {
  std::string input = "(2 + 2) * 2 - 99 / 9 + 24 * ((12-2) * 9)";

  std::cout << execute_expression(input) << std::endl;
}