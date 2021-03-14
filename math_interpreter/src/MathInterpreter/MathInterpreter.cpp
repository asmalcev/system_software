#include "MathInterpreter.hpp"

#include <iomanip>
#include <sstream>

static void print_list(std::list<math_token> * plist, std::string spliter = "");

static token_type char_type(char c) {
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
  return token_type::none;
}

static bool is_action_type(token_type tt) {
  return tt == token_type::action_plus ||
         tt == token_type::action_minus ||
         tt == token_type::action_multiply ||
         tt == token_type::action_divide;
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
  token_type tt = token_type::none,
             token_tmp;

  for (auto c : str) {
    token_tmp = char_type(c);

    if (tt == token_type::none) {
      tt = token_tmp;
    } else {
      if (
        tt != token_type::number ||
        (tt != token_tmp && tt != token_type::float_number)
      ) {
        // if prev is float and current is number -> continue
        if (!(tt == token_type::float_number && token_tmp == token_type::number)) {
          if (token_tmp == token_type::float_point) {
            if (tt == token_type::float_number) {
              // if number is already float - new float point = error
              throw std::logic_error("Recurring float point in string");
            } else {
              if (tt == token_type::number) {
                tt = token_type::float_number;
              } else {
                plist->push_back({
                  value: str.substr(bindex, i - bindex),
                  type : tt
                });
                tt = float_number;
                bindex = i;
              }
            }
          } else {
            plist->push_back({
              value: str.substr(bindex, i - bindex),
              type : tt
            });
            tt = token_tmp;
            bindex = i;
          }
        }
      }
    }

    i++;
  }
  plist->push_back({
    value: str.substr(bindex),
    type : tt
  });

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

static std::string to_precision_string(Float n, size_t precision = 10) {
  std::stringstream ss;
  ss << std::setprecision(precision) << n;
  return ss.str();
}

std::string execute_expression(std::string input) {
  std::list<math_token> * plist = split_into_tokens(clear_spaces(input));

  PriorityStack<priority_token> action_stack;
  size_t bracket_depth = 0;

  for (auto it = plist->begin(); it != plist->end(); ++it) {
    if (it->type == token_type::bracket_open) {
      bracket_depth++;
    } else if (it->type == token_type::bracket_close) {
      bracket_depth--;
    } else if (it->type == token_type::action_plus) {
      if (it == plist->begin() || is_action_type((--it)->type)) {
        // if unary plus
        if (it != plist->begin()) {
          // if prev token exists need to iterate back to plus
          ++it;
        }
        it->type = token_type::action_unary_plus;
        action_stack.push({
          priority: 3 * bracket_depth + 2,
          it: it
        });
      } else {
        // if binary plus
        action_stack.push({
          priority: 3 * bracket_depth,
          it: (++it) // iterate to next token, because iterated to prev to check type
        });
      }
    } else if (it->type == token_type::action_minus) {
      if (it == plist->begin() || is_action_type((--it)->type)) {
        // if unary minus
        if (it != plist->begin()) {
          // if prev token exists need to iterate back to minus
          ++it;
        }
        it->type = token_type::action_unary_minus;
        action_stack.push({
          priority: 3 * bracket_depth + 2,
          it: it
        });
      } else {
        // if binary minus
        action_stack.push({
          priority: 3 * bracket_depth,
          it: (++it) // iterate to next token, because iterated to prev to check type
        });
      }
    } else if (it->type == token_type::action_multiply ||
               it->type == token_type::action_divide) {
      // if binary multiply or divide
      action_stack.push({
        priority: 3 * bracket_depth + 1,
        it: it
      });
    }
  }
  // print_list(plist, "\n");

  priority_token tmp;
  std::list<math_token>::iterator it;
  char surroundings;
  while (!action_stack.isEmpty()) {
    tmp = action_stack.pull();

    print_list(plist, " ");

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
        it->value = to_precision_string(n1 + n2);
      } else {
        throw std::logic_error("Operands of + must be numbers or floats");
      }

      // if high priority then it can be inro brackets
      if (tmp.priority > 2 && check_brackets(it)) {
        it = plist->erase(--it);
        it = plist->erase(++it);
      }
    
    // ACTION UNARY PLUS
    } else if (action == token_type::action_unary_plus) {
      surroundings = unary_surroundings_check(it);
      if (surroundings == 'a') {
        throw std::logic_error("Operand of + must be number or float");
      }

      // if high priority then it can be inro brackets
      if (tmp.priority > 2 && check_brackets(it)) {
        it = plist->erase(--it);
        it = plist->erase(++it);
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
        it->value = to_precision_string(n1 - n2);
      } else {
        throw std::logic_error("Operands of - must be numbers or floats");
      }

      // if high priority then it can be inro brackets
      if (tmp.priority > 2 && check_brackets(it)) {
        it = plist->erase(--it);
        it = plist->erase(++it);
      }

    // ACTION UNARY MINUS
    } else if (action == token_type::action_unary_minus) {
      surroundings = unary_surroundings_check(it);
      if (surroundings == 'n') {
        Number n1 = std::stoll(it->value);
        it->value = std::to_string(-n1);
      } else if (surroundings == 'f') {
        Float n1 = std::stold(it->value);
        it->value = to_precision_string(-n1);
      } else {
        throw std::logic_error("Operand of - must be number or float");
      }

      // if high priority then it can be inro brackets
      if (tmp.priority > 2 && check_brackets(it)) {
        it = plist->erase(--it);
        it = plist->erase(++it);
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
        it->value = to_precision_string(n1 * n2);
      } else {
        throw std::logic_error("Operands of * must be numbers or floats");
      }

      // if high priority then it can be inro brackets
      if (tmp.priority > 2 && check_brackets(it)) {
        it = plist->erase(--it);
        it = plist->erase(++it);
      }

    // ACTION DIVIDE
    } else if (action == token_type::action_divide) {
      surroundings = binary_surroundings_check(it);
      --it;
      if (surroundings == 'n') {
        Number n1 = std::stoll(it->value);
        it = plist->erase(it);
        Number n2 = std::stoll(it->value);
        it->value = std::to_string(n1 / n2);
      } else if (surroundings == 'f') {
        Float n1 = std::stold(it->value);
        it = plist->erase(it);
        Float n2 = std::stold(it->value);
        it->value = to_precision_string(n1 / n2);
      } else {
        throw std::logic_error("Operands of / must be numbers or floats");
      }

      // if high priority then it can be inro brackets
      if (tmp.priority > 2 && check_brackets(it)) {
        it = plist->erase(--it);
        it = plist->erase(++it);
      }
    }
  }
  
  std::string result = plist->begin()->value;
  delete plist;

  return result;
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