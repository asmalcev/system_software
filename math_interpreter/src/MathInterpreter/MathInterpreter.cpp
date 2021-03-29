#include "MathInterpreter.hpp"

#include <sstream>

static void print_list(std::list<math_token> * plist, std::string spliter = "");

token_type char_type_math(char c) {
  if (isdigit(c)) {
    return token_type::number;
  }
  if (c == '.') {
    return token_type::float_point;
  }
  if (c == '+') {
    return token_type::action_plus;
  }
  if (c == '-') {
    return token_type::action_minus;
  }
  if (c == '*') {
    return token_type::action_multiply;
  }
  if (c == '/') {
    return token_type::action_divide;
  }
  if (c == '(') {
    return token_type::bracket_open;
  }
  if (c == ')') {
    return token_type::bracket_close;
  }
  if (c == '<') {
    return token_type::logic_action_less;
  }
  if (c == '>') {
    return token_type::logic_action_more;
  }
  if (c == '&') {
    return token_type::logic_action_and;
  }
  if (c == '|') {
    return token_type::logic_action_or;
  }
  if (c == '!') {
    return token_type::logic_action_not;
  }
  if (c == '=') {
    return token_type::logic_action_equals;
  }
  if (isalpha(c)) {
    return token_type::symbol;
  }
  return token_type::none;
}

static bool can_be_unary_neighbor(token_type tt) {
  return tt == token_type::action_divide       ||
         tt == token_type::action_minus        ||
         tt == token_type::action_plus         ||
         tt == token_type::action_multiply     ||
         tt == token_type::logic_action_and    ||
         tt == token_type::logic_action_equals ||
         tt == token_type::logic_action_less   ||
         tt == token_type::logic_action_more   ||
         tt == token_type::logic_action_not    ||
         tt == token_type::logic_action_or     ||
         tt == token_type::bracket_open;
}

static std::string clear_spaces(std::string str) {
  std::stringstream ss;
  for (auto c : str) {
    if (!isspace(c)) {
      ss << c;
    }
  }
  return ss.str();
}

static std::list<math_token> * split_into_tokens(std::string str) {
  std::list<math_token> * plist = new std::list<math_token>();

  size_t bindex = 0,
         i      = 0;
  char c        = 0;
  bool should_push = true;
  token_type tt = token_type::none,
             token_tmp;

  for (; i < str.size(); ++i) {
    c = str[i];
    token_tmp = char_type_math(c);
    if (token_tmp == token_type::none) {
      throw std::invalid_argument("Unidentified symbol");
    }

    if (token_tmp == token_type::symbol) {
      if (i != 0) {
        if (should_push) {
          plist->push_back({
            value: str.substr(bindex, i - bindex),
            type : tt
          });
        }

        tt = token_type::symbol;
        bindex = i;
      }

      size_t functionLength = 0;
      plist->push_back(_execute_function(str.substr(bindex), functionLength));
      i += functionLength - (i == 0 ? 2 : 1);

      should_push = false;

    } else if (tt == token_type::none) {
      tt = token_tmp;
    } else {
      if (
        (tt == token_type::logic_action_or && token_tmp == token_type::logic_action_or) ||
        (tt == token_type::logic_action_and && token_tmp == token_type::logic_action_and) ||
        (tt == token_type::logic_action_equals && token_tmp == token_type::logic_action_equals)
      ) {
        if (i - bindex + 1 > 2) {
          throw std::logic_error("Incomprehensible sequence: " + str.substr(bindex, i - bindex + 1));
        }
      } else if (
        tt != token_type::number ||
        (tt != token_tmp && tt != token_type::float_number)
      ) {
        // if prev is float and current is number -> continue
        if (!(tt == token_type::float_number && token_tmp == token_type::number)) {
          if (token_tmp == token_type::float_point) {
            if (tt == token_type::float_number) {
              // if number is already float - new float point = error
              throw std::logic_error("Recurring float point in number");
            } else {
              if (tt == token_type::number) {
                tt = token_type::float_number;
              } else {
                if (should_push) plist->push_back({
                  value: str.substr(bindex, i - bindex),
                  type : tt
                });

                tt          = token_type::float_number;
                bindex      = i;
                should_push = true;
              }
            }
          } else {
            if (tt == token_type::float_number && i - bindex == 1) {
              if (should_push) plist->push_back({
                value: "0",
                type : tt
              });
            } else {
              if (should_push) plist->push_back({
                value: str.substr(bindex, i - bindex),
                type : tt
              });
            }
            tt          = token_tmp;
            bindex      = i;
            should_push = true;
          }
        }
      }
    }
  }
  if (tt == token_type::float_number && i - bindex == 1) {
    if (should_push) plist->push_back({
      value: "0",
      type : tt
    });
  } else {
    if (should_push) plist->push_back({
      value: str.substr(bindex, i - bindex),
      type : tt
    });
  }

  return plist;
}

static bool check_brackets(std::list<math_token>::iterator it) {
  return (--it)->type == token_type::bracket_open &&
         (++(++it))->type == token_type::bracket_close;
}

static char unary_surroundings_check(std::list<math_token>::iterator it) {
  if (it->type == token_type::number) {
    return 'n';
  } else if (it->type == token_type::float_number) {
    return 'f';
  } else {
    // another type
    return 'a';
  }
}

static char binary_surroundings_check(std::list<math_token>::iterator it) {
  char tmp = unary_surroundings_check(it);
  if ((--it)->type == token_type::number) {
    return tmp == 'f' ? 'f' : 'n';
  } else if (it->type == token_type::float_number) {
    return 'f';
  } else {
    // another type
    return 'a';
  }
}

math_token _execute_expression(std::string input) {
  std::list<math_token> * plist = split_into_tokens(clear_spaces(input));

  PriorityStack<priority_token> action_stack;
  size_t bracket_depth = 0;

  // print_list(plist, "\n");

  for (auto it = plist->begin(); it != plist->end(); ++it) {
    if (it->type == token_type::bracket_open) {
      bracket_depth++;
    } else if (it->type == token_type::bracket_close) {
      if (bracket_depth == 0) {
        throw std::logic_error("Superfluous brackets ')' in math expression");
      }
      bracket_depth--;
    } else if (it->type == token_type::action_plus) {
      if (it == plist->begin() || can_be_unary_neighbor((--it)->type)) {
        // if unary plus
        if (it != plist->begin()) {
          // if prev token exists need to iterate back to plus
          ++it;
        }
        it->type = token_type::action_unary_plus;
        action_stack.push({
          priority: PRIORITY_NOTATION * bracket_depth + 6,
          it: it
        });
      } else {
        // if binary plus
        action_stack.push({
          priority: PRIORITY_NOTATION * bracket_depth + 4,
          it: (++it) // iterate to next token, because iterated to prev to check type
        });
      }
    } else if (it->type == token_type::action_minus) {
      if (it == plist->begin() || can_be_unary_neighbor((--it)->type)) {
        // if unary minus
        if (it != plist->begin()) {
          // if prev token exists need to iterate back to minus
          ++it;
        }
        it->type = token_type::action_unary_minus;
        action_stack.push({
          priority: PRIORITY_NOTATION * bracket_depth + 6,
          it: it
        });
      } else {
        // if binary minus
        action_stack.push({
          priority: PRIORITY_NOTATION * bracket_depth + 4,
          it: (++it) // iterate to next token, because iterated to prev to check type
        });
      }
    } else if (it->type == token_type::action_multiply ||
               it->type == token_type::action_divide) {
      // if binary multiply or divide
      action_stack.push({
        priority: PRIORITY_NOTATION * bracket_depth + 5,
        it: it
      });
    } else if (it->type == token_type::logic_action_or) {
      if (it->value.size() != 2) {
        throw std::logic_error("Unsupported operator: " + it->value);
      }
      action_stack.push({
        priority: PRIORITY_NOTATION * bracket_depth,
        it: it
      });
    } else if (it->type == token_type::logic_action_and) {
      if (it->value.size() != 2) {
        throw std::logic_error("Unsupported operator: " + it->value);
      }
      action_stack.push({
        priority: PRIORITY_NOTATION * bracket_depth + 1,
        it: it
      });
    } else if (it->type == token_type::logic_action_equals) {
      action_stack.push({
        priority: PRIORITY_NOTATION * bracket_depth + 2,
        it: it
      });
    } else if (it->type == token_type::logic_action_not) {
      action_stack.push({
        priority: PRIORITY_NOTATION * bracket_depth + 7,
        it: it
      });
    } else if (it->type == token_type::logic_action_less ||
               it->type == token_type::logic_action_more) {
      action_stack.push({
        priority: PRIORITY_NOTATION * bracket_depth + 3,
        it: it
      });
    }
  }

  if (bracket_depth > 0) {
    throw std::logic_error("Superfluous brackets '('");
  }

  for (auto it = plist->begin(); it != plist->end();) {
    if (it->type == token_type::bracket_open ||
        it->type == token_type::bracket_close) {
      it = plist->erase(it);
    } else {
      ++it;
    }
  }

  priority_token tmp;
  std::list<math_token>::iterator it;
  char surroundings;
  while (!action_stack.isEmpty()) {
    tmp = action_stack.pull();

    // print_list(plist, " ");
    // std::cout << tmp << std::endl;

    token_type action = tmp.it->type;
    it = plist->erase(tmp.it);

    // ACTION PLUS
    if (action == token_type::action_plus) {
      surroundings = binary_surroundings_check(it);
      --it;
      if (surroundings == 'n') {
        Number n1 = std::stoll(it->value);
        it = plist->erase(it);
        Number n2 = std::stoll(it->value);
        it->value = std::to_string(n1 + n2);
      } else if (surroundings == 'f') {
        Float n1 = std::stold(it->value);
        it = plist->erase(it);
        Float n2 = std::stold(it->value);
        it->value = std::to_string(n1 + n2);
      } else {
        throw std::logic_error("Operands of + must be numbers or floats");
      }

    // ACTION UNARY PLUS
    } else if (action == token_type::action_unary_plus) {
      surroundings = unary_surroundings_check(it);
      if (surroundings == 'a') {
        throw std::logic_error("Operand of + must be number or float");
      }

    // ACTION MINUS
    } else if (action == token_type::action_minus) {
      surroundings = binary_surroundings_check(it);
      --it;
      if (surroundings == 'n') {
        Number n1 = std::stoll(it->value);
        it = plist->erase(it);
        Number n2 = std::stoll(it->value);
        it->value = std::to_string(n1 - n2);
      } else if (surroundings == 'f') {
        Float n1 = std::stold(it->value);
        it = plist->erase(it);
        Float n2 = std::stold(it->value);
        it->value = std::to_string(n1 - n2);
      } else {
        throw std::logic_error("Operands of - must be numbers or floats");
      }

    // ACTION UNARY MINUS
    } else if (action == token_type::action_unary_minus) {
      surroundings = unary_surroundings_check(it);
      if (surroundings == 'n') {
        Number n1 = std::stoll(it->value);
        it->value = std::to_string(-n1);
      } else if (surroundings == 'f') {
        Float n1 = std::stold(it->value);
        it->value = std::to_string(-n1);
      } else {
        throw std::logic_error("Operand of - must be number or float");
      }

    // ACTION MULTIPLY
    } else if (action == token_type::action_multiply) {
      surroundings = binary_surroundings_check(it);
      --it;
      if (surroundings == 'n') {
        Number n1 = std::stoll(it->value);
        it = plist->erase(it);
        Number n2 = std::stoll(it->value);
        it->value = std::to_string(n1 * n2);
      } else if (surroundings == 'f') {
        Float n1 = std::stold(it->value);
        it = plist->erase(it);
        Float n2 = std::stold(it->value);
        it->value = std::to_string(n1 * n2);
      } else {
        throw std::logic_error("Operands of * must be numbers or floats");
      }

    // ACTION DIVIDE
    } else if (action == token_type::action_divide) {
      surroundings = binary_surroundings_check(it);
      --it;
      if (surroundings == 'n') {
        Number n1 = std::stoll(it->value);
        it = plist->erase(it);
        Number n2 = std::stoll(it->value);
        if (n2 == 0) {
          throw std::logic_error("Can't divide by 0");
        }
        it->value = std::to_string(n1 / n2);
      } else if (surroundings == 'f') {
        Float n1 = std::stold(it->value);
        it = plist->erase(it);
        Float n2 = std::stold(it->value);
        if (n2 < __DBL_EPSILON__) {
          throw std::logic_error("Can't divide by 0");
        }
        it->value = std::to_string(n1 / n2);
      } else {
        throw std::logic_error("Operands of / must be numbers or floats");
      }

    // LOGIC >
    } else if (action == token_type::logic_action_more) {
      surroundings = binary_surroundings_check(it);
      --it;
      if (surroundings == 'n') {
        Number n1 = std::stoll(it->value);
        it = plist->erase(it);
        Number n2 = std::stoll(it->value);
        it->value = std::to_string(n1 > n2);
      } else if (surroundings == 'f') {
        Float n1 = std::stold(it->value);
        it = plist->erase(it);
        Float n2 = std::stold(it->value);
        it->value = std::to_string(n1 > n2);
      } else {
        throw std::logic_error("Operands of > must be numbers or floats");
      }

    // LOGIC <
    } else if (action == token_type::logic_action_less) {
      surroundings = binary_surroundings_check(it);
      --it;
      if (surroundings == 'n') {
        Number n1 = std::stoll(it->value);
        it = plist->erase(it);
        Number n2 = std::stoll(it->value);
        it->value = std::to_string(n1 < n2);
      } else if (surroundings == 'f') {
        Float n1 = std::stold(it->value);
        it = plist->erase(it);
        Float n2 = std::stold(it->value);
        it->value = std::to_string(n1 < n2);
      } else {
        throw std::logic_error("Operands of < must be numbers or floats");
      }

    // LOGIC !
    } else if (action == token_type::logic_action_not) {
      surroundings = unary_surroundings_check(it);
      if (surroundings == 'n') {
        Number n1 = std::stoll(it->value);
        it->value = std::to_string(!n1);
      } else {
        throw std::logic_error("Operand of ! must be number");
      }

    // LOGIC ==
    } else if (action == token_type::logic_action_equals) {
      surroundings = binary_surroundings_check(it);
      --it;
      if (surroundings == 'n') {
        Number n1 = std::stoll(it->value);
        it = plist->erase(it);
        Number n2 = std::stoll(it->value);
        it->value = std::to_string(n1 == n2);
      } else if (surroundings == 'f') {
        Float n1 = std::stold(it->value);
        it = plist->erase(it);
        Float n2 = std::stold(it->value);
        it->value = std::to_string(n1 == n2);
      } else {
        throw std::logic_error("Operands of == must be numbers or floats");
      }

    // LOGIC &&
    } else if (action == token_type::logic_action_and) {
      surroundings = binary_surroundings_check(it);
      --it;
      if (surroundings == 'n') {
        Number n1 = std::stoll(it->value);
        it = plist->erase(it);
        Number n2 = std::stoll(it->value);
        it->value = std::to_string(n1 && n2);
      } else if (surroundings == 'f') {
        Float n1 = std::stold(it->value);
        it = plist->erase(it);
        Float n2 = std::stold(it->value);
        it->value = std::to_string(n1 && n2);
      } else {
        throw std::logic_error("Operands of && must be numbers or floats");
      }

    // LOGIC ||
    } else if (action == token_type::logic_action_or) {
      surroundings = binary_surroundings_check(it);
      --it;
      if (surroundings == 'n') {
        Number n1 = std::stoll(it->value);
        it = plist->erase(it);
        Number n2 = std::stoll(it->value);
        it->value = std::to_string(n1 || n2);
      } else if (surroundings == 'f') {
        Float n1 = std::stold(it->value);
        it = plist->erase(it);
        Float n2 = std::stold(it->value);
        it->value = std::to_string(n1 || n2);
      } else {
        throw std::logic_error("Operands of || must be numbers or floats");
      }

    }
  }

  math_token result = *plist->begin();
  delete plist;

  return result;
}

std::string execute_expression(std::string input) {
  return _execute_expression(input).value;
}




/**************************************************************
 *
 *                           UTILS
 *
**************************************************************/
static void print_list(std::list<math_token> * plist, std::string spliter) {
  for (auto it = plist->begin(); it != plist->end(); ++it) {
    std::cout << it->value << spliter;
  }
  std::cout << std::endl;
}

std::ostream& operator<<(std::ostream& os, const priority_token& pt) {
  return (os << pt.priority << " " << pt.it->value);
}