#include <iostream>
#include <fstream>
#include <string>

#include "MathInterpreter/MathInterpreter.hpp"

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

  size_t lineIndex = 1;
  std::string result;
  while (std::getline(input, line)) {
    try {
      result = execute_expression(line);
      std::cout << line << " = " << result << std::endl;
    } catch(std::logic_error err) {
      std::cout << "Error on " << lineIndex << " line: " << err.what() << std::endl;
    }
    lineIndex++;
  }
}