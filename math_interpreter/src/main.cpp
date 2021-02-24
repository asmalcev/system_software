#include <iostream>
#include <string>

#include "LinkedList/LinkedList.hpp"

enum token_type {
  none,
  number,
  action_plus,
  action_minus,
  action_multiply,
  action_divide,
  bracket_open,
  bracket_close,
  space
};

token_type char_type(char c) {
  if (isspace(c)) {
    return token_type::space;
  }
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

struct math_token {
  std::string value;
  token_type  type;
};

LinkedList<math_token> * split_into_tokens(std::string str) {
  LinkedList<math_token> * plist = new LinkedList<math_token>();

  size_t bindex = 0,
         i      = 0;
  token_type tt = token_type::none,
             token_tmp;

  for (auto c : str) {
    token_tmp = char_type(c);
    if (token_tmp == token_type::space) {
      i++;
      continue;
    }

    if (tt == token_type::none) {
      tt = token_tmp;
    } else {
      if (tt != token_tmp) {
        plist->pushNode({
          value: str.substr(bindex, i - bindex),
          type : tt
        });
        tt = token_tmp;
        bindex = i;
      }
    }

    i++;
  }
  plist->pushNode({
    value: str.substr(bindex),
    type : tt
  });

  return plist;
}

int main() {
  std::string input = "7 + 12 * 48 - (22 / 2)";

  LinkedList<math_token> * plist = split_into_tokens(input);
  LinkedList<math_token>::iterator high_priority = plist->begin();
  size_t bracket_depth = 0, last_bracket_depth = 0;

  for (auto it = plist->begin(); it != plist->end(); ++it) {
    if (it->type == token_type::bracket_open) {
      bracket_depth++;
    } else if (it->type == token_type::bracket_close) {
      bracket_depth--;
    }
    // std::cout << (*it).value << " " << (*it).type << std::endl;
  }
}