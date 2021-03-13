#include "MathInterpreter.hpp"

static void print_list(std::list<math_token> * plist, std::string spliter = "");

static token_type char_type(char c) {
  if (isdigit(c)) {
    return token_type::number;
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

static bool isActionType(token_type tt) {
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
      if (tt != token_type::number || tt != token_tmp) {
        plist->push_back({
          value: str.substr(bindex, i - bindex),
          type : tt
        });
        tt = token_tmp;
        bindex = i;
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
      if (it == plist->begin() || isActionType((--it)->type)) {
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
      if (it == plist->begin() || isActionType((--it)->type)) {
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
  while (!action_stack.isEmpty()) {
    tmp = action_stack.pull();

    // print_list(plist, " ");
    // std::cout << "OPERATOR: " << tmp.priority << " " << tmp.it->value << std::endl;

    token_type action = tmp.it->type;
    it = plist->erase(tmp.it);
    
    // ACTION PLUS
    if (action == token_type::action_plus) {
      if (it->type == token_type::number &&
          (--it)->type == token_type::number) {
        Number n1 = atoi(it->value.c_str());
        it = plist->erase(it);
        Number n2 = atoi(it->value.c_str());
        it->value = std::to_string(n1 + n2);
        if (tmp.priority > 2 && check_brackets(it)) {
          it = plist->erase(--it);
          it = plist->erase(++it);
        }
      }
    
    // ACTION UNARY PLUS
    } else if (action == token_type::action_unary_plus) {
      if (it->type == token_type::number) {
        if (tmp.priority > 2 && check_brackets(it)) {
          it = plist->erase(--it);
          it = plist->erase(++it);
        }
      }

    // ACTION MINUS
    } else if (action == token_type::action_minus) {
      if (it->type == token_type::number &&
          (--it)->type == token_type::number) {
        Number n1 = atoi(it->value.c_str());
        it = plist->erase(it);
        Number n2 = atoi(it->value.c_str());
        it->value = std::to_string(n1 - n2);
        if (tmp.priority > 2 && check_brackets(it)) {
          it = plist->erase(--it);
          it = plist->erase(++it);
        }
      }

    // ACTION UNARY MINUS
    } else if (action == token_type::action_unary_minus) {
      if (it->type == token_type::number) {
        Number n1 = atoi(it->value.c_str());
        it->value = std::to_string(-n1);
        if (tmp.priority > 2 && check_brackets(it)) {
          it = plist->erase(--it);
          it = plist->erase(++it);
        }
      }

    // ACTION MULTIPLY
    } else if (action == token_type::action_multiply) {
      if (it->type == token_type::number &&
          (--it)->type == token_type::number) {
        Number n1 = atoi(it->value.c_str());
        it = plist->erase(it);
        Number n2 = atoi(it->value.c_str());
        it->value = std::to_string(n1 * n2);
        if (tmp.priority > 2 && check_brackets(it)) {
          it = plist->erase(--it);
          it = plist->erase(++it);
        }
      }

    // ACTION DIVIDE
    } else if (action == token_type::action_divide) {
      if (it->type == token_type::number &&
          (--it)->type == token_type::number) {
        Number n1 = atoi(it->value.c_str());
        it = plist->erase(it);
        Number n2 = atoi(it->value.c_str());
        it->value = std::to_string(n1 / n2);
        if (tmp.priority > 2 && check_brackets(it)) {
          it = plist->erase(--it);
          it = plist->erase(++it);
        }
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