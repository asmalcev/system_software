#include <iostream>
#include <fstream>

#include "PseudoInterpreter/PseudoInterpreter.hpp"

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