#include <iostream>
#include <regex>
#include <sstream>

#include "../../math_interpreter/src/MathInterpreter/MathInterpreter.hpp"
#include "../../math_interpreter/src/FunctionInterpreter/FunctionInterpreter.hpp"

std::string clear_input(std::string input) {
  std::stringstream ss;
  for (auto c : input) {
    if (c != '\n' &&
        c != ' ') {
      ss << c;
    }
  }
  return ss.str();
}

int main() {
  std::string input = clear_input("if (4) {\nprint(4+4);\n} else {\nif (3+2) {\nprint(2);\n}\nprint(-4);\n}");

  std::regex re("if|else|[\\{\\}]");
  std::string buffer;
  size_t bindex        = 0,
         cur_pos       = 0,
         bracket_depth = 0;

  for (
    std::sregex_iterator it = std::sregex_iterator(input.begin(), input.end(), re);
    it != std::sregex_iterator();
    it++
  ) {
    buffer  = (*it).str(0);
    cur_pos = (*it).position();

    if (buffer == "if") {
      bindex = cur_pos + 2;
      std::cout << bracket_depth << "_if ";

    } else if (buffer == "{") {
      std::cout << input.substr(bindex, cur_pos - bindex) << " " << bracket_depth << "_{\n";
      bindex = cur_pos + 1;

      bracket_depth++;

    } else if (buffer == "}") {
      if (bracket_depth == 0) {
        throw std::logic_error("Superfluous brackets }");
      }
      bracket_depth--;

      std::cout << input.substr(bindex, cur_pos - bindex) << "\n" << bracket_depth << "_}\n";
      bindex = cur_pos + 1;

    } else if (buffer == "else") {
      bindex = cur_pos + 4;
      std::cout << bracket_depth << "_else ";
    }
  }
  if (bracket_depth != 0) {
    throw std::logic_error("Superfluous brackets {");
  }
}