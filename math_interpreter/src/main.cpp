#include <iostream>
#include <string>

#include "LinkedList/LinkedList.hpp"

enum token_type {
  none,
  number,
  action,
  space
};

token_type char_type(char c) {
  if (isspace(c)) {
    return token_type::space;
  }
  if (isdigit(c)) {
    return token_type::number;
  }
  if (c == '+' ||
      c == '-' ||
      c == '*' ||
      c == '/') {
    return token_type::action;
  }
}

int main() {
  std::string input = "7 + 12 * 48";

  size_t bindex = 0,
         i      = 0;
  token_type tt = token_type::none,
             token_tmp;

  for (auto c : input) {
    token_tmp = char_type(c);
    if (token_tmp == token_type::space) {
      i++;
      continue;
    }

    if (tt == token_type::none) {
      tt = token_tmp;
    } else {
      if (tt != token_tmp) {
        tt = token_tmp;
        std::cout << input.substr(bindex, i - bindex) << std::endl;
        bindex = i;
      }
    }

    i++;
  }
  std::cout << input.substr(bindex) << std::endl;
}