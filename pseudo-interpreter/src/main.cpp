#include <iostream>
#include <regex>
#include <sstream>
#include <stack>

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

bool check_logic_expression(std::string str) {
  math_token result = _execute_expression(str);
  if (result.type == token_type::number) {
    return (bool) std::stoll(result.value);
  }
  if (result.type == token_type::float_number) {
    return (bool) std::stold(result.value);
  }
  return false;
}

enum operator_token {
  t_if,
  t_else
};

struct condition_state {
  operator_token op_t;
  size_t depth;
  bool state;
};

void execute_fragment(std::string str) {
  std::regex re(";");
  size_t bindex  = 0,
         cur_pos = 0;
  for (
    std::sregex_iterator it = std::sregex_iterator(str.begin(), str.end(), re);
    it != std::sregex_iterator();
    it++
  ) {
    cur_pos = (*it).position();
    execute_expression(str.substr(bindex, cur_pos - bindex));
    bindex = cur_pos + 1;
  }
  if (cur_pos + 1 != str.size()) {
    throw std::logic_error("Missed ; symbol");
  }
}

int main() {
  std::string input = clear_input("if (4 > 5) {\nprintln(4 + 4);\n} else {\nprintln(-4 * 6);\n}");

  std::regex re("if|else|[\\{\\}]");
  std::string buffer;
  size_t bindex        = 0,
         cur_pos       = 0,
         bracket_depth = 0;

  std::stack<condition_state> history;

  for (
    std::sregex_iterator it = std::sregex_iterator(input.begin(), input.end(), re);
    it != std::sregex_iterator();
    it++
  ) {
    buffer  = (*it).str(0);
    cur_pos = (*it).position();

    if (buffer == "if") {
      bindex = cur_pos + 2;
      history.push({operator_token::t_if, bracket_depth});

    } else if (buffer == "{") {
      if (history.top().op_t == operator_token::t_if) {
        history.top().state = check_logic_expression(input.substr(bindex, cur_pos - bindex));
      }

      bindex = cur_pos + 1;
      bracket_depth++;

    } else if (buffer == "}") {
      if (bracket_depth == 0) {
        throw std::logic_error("Superfluous brackets }");
      }
      bracket_depth--;
      if (history.top().state) {
        execute_fragment(input.substr(bindex, cur_pos - bindex));
      }

      bindex = cur_pos + 1;

    } else if (buffer == "else") {
      bindex = cur_pos + 4;
      history.push({operator_token::t_else, bracket_depth, !history.top().state});
    }
  }
  if (bracket_depth != 0) {
    throw std::logic_error("Superfluous brackets {");
  }
}