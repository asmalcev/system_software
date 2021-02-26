#include <iostream>
#include <string>

#include "LinkedList/LinkedList.cpp"
#include "Stack/PriorityStack.cpp"

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

struct priority_token {
  size_t priority;
  LinkedList<math_token>::iterator it;

  inline bool operator<(const priority_token& rhs) const {
    return priority < rhs.priority;
  }
};

typedef long long Number;

bool check_brackets(LinkedList<math_token>::iterator it) {
  return (--it)->type == token_type::bracket_open &&
         (++(++it))->type == token_type::bracket_close;
}

int main() {
  std::string input = "(2 + 2) * 2";

  LinkedList<math_token> * plist = split_into_tokens(input);
  size_t bracket_depth = 0;

  // for (auto it = plist->begin(); it != plist->end(); ++it) {
  //   std::cout << it->value << std::endl;
  // }

  PriorityStack<priority_token> action_stack;

  for (auto it = plist->begin(); it != plist->end(); ++it) {
    if (it->type == token_type::bracket_open) {
      bracket_depth++;
    } else if (it->type == token_type::bracket_close) {
      bracket_depth--;
    } else if (it->type == token_type::action_plus ||
               it->type == token_type::action_minus) {
      action_stack.push({
        priority: 2 * bracket_depth,
        it: it
      });
    } else if (it->type == token_type::action_multiply ||
               it->type == token_type::action_divide) {
      action_stack.push({
        priority: 2 * bracket_depth + 1,
        it: it
      });
    }
  }

  priority_token tmp;
  LinkedList<math_token>::iterator it;
  while (!action_stack.isEmpty()) {
    tmp = action_stack.pull();
    // std::cout << "OPERATOR: " << tmp.priority << " " << tmp.it->value << std::endl;
    
    token_type action = tmp.it->type;
    it = plist->remove(tmp.it);
    if (action == token_type::action_plus) {
      if (it->type == token_type::number &&
          (--it)->type == token_type::number) {
        Number n1 = atoi(it->value.c_str());
        it = plist->remove(it);
        Number n2 = atoi(it->value.c_str());
        if (tmp.priority > 1 && check_brackets(it)) {
          it = plist->remove(--it);
          it = plist->remove(++it);
        }
        // std::cout << n1 << " + " << n2 << " = " << (n1 + n2) << std::endl;
        it->value = std::to_string(n1 + n2);
      }
    } else if (action == token_type::action_minus) {
      if (it->type == token_type::number &&
          (--it)->type == token_type::number) {
        Number n1 = atoi(it->value.c_str());
        it = plist->remove(it);
        Number n2 = atoi(it->value.c_str());
        if (tmp.priority > 1 && check_brackets(it)) {
          it = plist->remove(--it);
          it = plist->remove(++it);
        }
        // std::cout << n1 << " - " << n2 << " = " << (n1 - n2) << std::endl;
        it->value = std::to_string(n1 - n2);
      }
    } else if (action == token_type::action_multiply) {
      if (it->type == token_type::number &&
          (--it)->type == token_type::number) {
        Number n1 = atoi(it->value.c_str());
        it = plist->remove(it);
        Number n2 = atoi(it->value.c_str());
        if (tmp.priority > 1 && check_brackets(it)) {
          it = plist->remove(--it);
          it = plist->remove(++it);
        }
        // std::cout << n1 << " * " << n2 << " = " << (n1 * n2) << std::endl;
        it->value = std::to_string(n1 * n2);
      }
    } else if (action == token_type::action_divide) {
      if (it->type == token_type::number &&
          (--it)->type == token_type::number) {
        Number n1 = atoi(it->value.c_str());
        it = plist->remove(it);
        Number n2 = atoi(it->value.c_str());
        if (tmp.priority > 1 && check_brackets(it)) {
          it = plist->remove(--it);
          it = plist->remove(++it);
        }
        // std::cout << n1 << " / " << n2 << " = " << (n1 / n2) << std::endl;
        it->value = std::to_string(n1 / n2);
      }
    }
  }

  for (auto it = plist->begin(); it != plist->end(); ++it) {
    std::cout << it->value << " ";
  }
  std::cout << std::endl;
}