#include "FunctionInterpreter.hpp"

#include <sstream>
#include <cmath>

token_type char_type_func(char c) {
  if (c == ',') {
    return token_type::comma;
  }
  if (c == '(') {
    return token_type::bracket_open;
  }
  if (c == ')') {
    return token_type::bracket_close;
  }
  if (c == '{') {
    return token_type::figured_bracket_open;
  }
  if (c == '}') {
    return token_type::figured_bracket_close;
  }
  return token_type::none;
}

supported_functions detect_function(std::string func_name) {
  if (func_name == "sin") {
    return supported_functions::f_sin;
  }
  if (func_name == "cos") {
    return supported_functions::f_cos;
  }
  if (func_name == "tan") {
    return supported_functions::f_tan;
  }
  if (func_name == "asin") {
    return supported_functions::f_asin;
  }
  if (func_name == "acos") {
    return supported_functions::f_acos;
  }
  if (func_name == "atan") {
    return supported_functions::f_atan;
  }
  if (func_name == "println") {
    return supported_functions::f_println;
  }
  return supported_functions::unindefined;
}

struct math_token _execute_function(std::string str, size_t & p_length) {
  size_t bracket_depth = 1,
         bindex        = 0;
  char c               = 0;
  token_type tt;
  std::list<math_token> args;

  while (str[p_length++] != '(' && p_length < str.size());
  if (p_length >= str.size()) {
    throw std::logic_error("The function was not called");
  }
  supported_functions sf = detect_function(str.substr(0, p_length - 1));
  if (sf == supported_functions::unindefined) {
    throw std::invalid_argument("Unindefined function name - " + str.substr(0, p_length - 1));
  }

  bindex = p_length;
  size_t tmp = bindex;
  do {
    c  = str[p_length++];
    tt = char_type_func(c);

    if (tt == token_type::bracket_open) {
      bracket_depth++;
    } else if (tt == token_type::bracket_close) {
      if (bracket_depth == 1) {
        args.push_back(
          _execute_expression(
            variable_controller::instance().fill_string_with_variables(
              str.substr(bindex, p_length - bindex - 1)
            )
          )
        );
      }
      bracket_depth--;
    }

    if (tt == token_type::comma && bracket_depth == 1) {
      args.push_back(
        _execute_expression(
          variable_controller::instance().fill_string_with_variables(
            str.substr(bindex, p_length - bindex - 1)
          )
        )
      );
      bindex = p_length;
    }

  } while(bracket_depth);

  std::string result;
  auto it = args.begin();
  if (sf == supported_functions::f_sin) {
    if (args.size() != 1) {
      throw std::invalid_argument("Invalid number of arguments for sin function");
    }
    if (it->type == token_type::number) {
      result = std::to_string(sin((Float) std::stoll(it->value)));
    } else if (it->type == token_type::float_number) {
      result = std::to_string(sin(std::stold(it->value)));
    }
    tt = token_type::float_number;
  } else if (sf == supported_functions::f_cos) {
    if (args.size() != 1) {
      throw std::invalid_argument("Invalid number of arguments for cos function");
    }
    if (it->type == token_type::number) {
      result = std::to_string(cos((Float) std::stoll(it->value)));
    } else if (it->type == token_type::float_number) {
      result = std::to_string(cos(std::stold(it->value)));
    }
    tt = token_type::float_number;
  } else if (sf == supported_functions::f_tan) {
    if (args.size() != 1) {
      throw std::invalid_argument("Invalid number of arguments for tan function");
    }
    if (it->type == token_type::number) {
      result = std::to_string(tan((Float) std::stoll(it->value)));
    } else if (it->type == token_type::float_number) {
      result = std::to_string(tan(std::stold(it->value)));
    }
    tt = token_type::float_number;
  } else if (sf == supported_functions::f_asin) {
    if (args.size() != 1) {
      throw std::invalid_argument("Invalid number of arguments for asin function");
    }
    if (it->type == token_type::number) {
      result = std::to_string(asin((Float) std::stoll(it->value)));
    } else if (it->type == token_type::float_number) {
      result = std::to_string(asin(std::stold(it->value)));
    }
    tt = token_type::float_number;
  } else if (sf == supported_functions::f_acos) {
    if (args.size() != 1) {
      throw std::invalid_argument("Invalid number of arguments for acos function");
    }
    if (it->type == token_type::number) {
      result = std::to_string(acos((Float) std::stoll(it->value)));
    } else if (it->type == token_type::float_number) {
      result = std::to_string(acos(std::stold(it->value)));
    }
    tt = token_type::float_number;
  } else if (sf == supported_functions::f_atan) {
    if (args.size() != 1) {
      throw std::invalid_argument("Invalid number of arguments for atan function");
    }
    if (it->type == token_type::number) {
      result = std::to_string(atan((Float) std::stoll(it->value)));
    } else if (it->type == token_type::float_number) {
      result = std::to_string(atan(std::stold(it->value)));
    }
    tt = token_type::float_number;
  } else if (sf == supported_functions::f_println) {
    if (args.size() == 0) {
      throw std::invalid_argument("Invalid number of arguments for println function");
    }
    for (; it != args.end(); ++it) {
      std::cout << it->value << " ";
    }
    std::cout << std::endl;
    result = "";
    tt     = token_type::none;
  }

  return {
    value: result,
    type: tt
  };
}

std::string execute_function(std::string str) {
  size_t string_size = 0;
  return _execute_function(str, string_size).value;
}