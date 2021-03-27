#pragma once

#include <iostream>
#include <string>
#include <list>

#include "../Stack/PriorityStack.hpp"
#include "../FunctionInterpreter/FunctionInterpreter.hpp"

enum token_type {
  none,
  number,
  float_number,
  action_plus,
  action_minus,
  action_unary_plus,
  action_unary_minus,
  action_multiply,
  action_divide,
  bracket_open,
  bracket_close,
  float_point,
  symbol,
  comma,
  logic_action_less,
  logic_action_more,
  logic_action_and, // not supported
  logic_action_or, // not supported
  logic_action_not,
  logic_action_equals, // not supported
};

constexpr size_t PRIORITY_NOTATION = 8;

struct math_token {
  std::string value;
  token_type  type;
};

struct priority_token {
  size_t priority;
  std::list<math_token>::iterator it;

  inline bool operator<(const priority_token& rhs) const {
    return priority < rhs.priority;
  }

  inline bool operator<=(const priority_token& rhs) const {
    return priority <= rhs.priority;
  }
};

typedef long long Number;
typedef long double Float;

math_token _execute_expression(std::string input);
std::string execute_expression(std::string input);
std::ostream& operator<<(std::ostream& os, const priority_token& pt);