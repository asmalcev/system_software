#include <iostream>
#include <string>

#include "MathInterpreter/MathInterpreter.hpp"

int main() {
  std::string input = "2 * 2.4 + 2 / .3 + - .1 + .";

  std::cout << execute_expression(input) << std::endl;
}