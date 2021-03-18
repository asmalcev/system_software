#include "FunctionInterpreter.hpp"

#include <cmath>

static auto functions = {
  sin
};

supported_functions detect_function(std::string func_name) {
  if (func_name == "sin") {
    return supported_functions::f_sin;
  }
  return supported_functions::unindefined;
}

struct math_token execute_function(std::string str, size_t & p_length) {
  size_t bracket_depth = 1,
         bindex        = 0;
  char c               = 0;
  token_type tt;
  std::list<argument> args;

  while (str[p_length++] != '(');
  supported_functions sf = detect_function(str.substr(0, p_length - 1));
  if (sf == supported_functions::unindefined) {
    throw std::invalid_argument("Unindefined function name");
  }

  do {
    c  = str[p_length++];
    tt = char_type(c);
    
    if (tt == token_type::bracket_open) {
      bracket_depth++;
    } else if (tt == token_type::bracket_close) {
      bracket_depth--;
    }

  } while(bracket_depth);

  return {
    // value: str,
    value: "1",
    type: token_type::number
  };
}
