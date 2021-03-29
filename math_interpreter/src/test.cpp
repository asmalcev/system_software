#include <iostream>

#include "MathInterpreter/MathInterpreter.hpp"

struct test {
  std::string input;
  std::string output;
};

constexpr size_t TEST_START_INDEX = 0;
constexpr size_t TESTS_COUNT = 29;

int main() {
  const test tests[] = {
    {
      "2 + 2 * 8 - 12",
      "6"
    },
    {
      "2 + 2 / 4 + 3 * 7",
      "23"
    },
    {
      "-4 + 2",
      "-2"
    },
    {
      "2 - - 4",
      "6"
    },
    {
      "(2 * 2) - 4",
      "0"
    },
    {
      "(2 + 2) * 2 - 99 / 9 + 24 * ((12-2) * 9)",
      "2157"
    },
    {
      "+4 * 10",
      "40"
    },
    {
      "(2 * 5)",
      "10"
    },
    {
      "2 * 2.4 + 2 / .3 + - .1",
      "11.366667"
    },
    {
      "4 + .",
      "4.000000"
    },
    {
      "2 / 0",
      "Can't divide by 0"
    },
    {
      ".2 / 0",
      "Can't divide by 0"
    },
    {
      "6 + * 3",
      "Operands of * must be numbers or floats"
    },
    {
      "10 + 1.24.2",
      "Recurring float point in number"
    },
    {
      "2 - 4..",
      "Recurring float point in number"
    },
    {
      "12.8 - (5 / 5.5 * (20 + 4))",
      "-9.018184"
    },
    {
      "4 + (10)",
      "14"
    },
    {
      "(((((((((412)))))))))",
      "412"
    },
    {
      "))))) 3 * 0  (((( + 2",
      "Superfluous brackets ')' in math expression"
    },
    {
      "sin(30 + 4) + 2",
      "2.529083"
    },
    {
      "5 * sin(30 + 4) + 2",
      "4.645415"
    },
    {
      "sin(1) + cos(1) + tan(1) + asin(1) + acos(1) + atan(1)",
      "5.295375"
    },
    {
      "sin 4 + 2",
      "The function was not called"
    },
    {
      "sin(3 * sin(4))",
      "-0.765094"
    },
    {
      "tan(4, 6)",
      "Invalid number of arguments for tan function"
    },
    {
      "create_array(1, 2, 3,4)",
      "Unindefined function name - create_array"
    },
    {
      "3 | 5",
      "Unsupported operator: |"
    },
    {
      "9 === 2",
      "Incomprehensible sequence: ==="
    },
    {
      "(3 && 0) == !1",
      "1"
    },
  };

  bool isOk;
  std::string answer;
  for (size_t i = TEST_START_INDEX; i < TESTS_COUNT; i++) {
    try {
      answer = execute_expression(tests[i].input);
      isOk   = answer == tests[i].output;

      if (isOk) {
        std::cout << "Test " << i + 1 << " passed" << std::endl;
      } else {
        std::cout << "Test " << i + 1 << " failed" << std::endl;
        std::cout << "Expression: " << tests[i].input << " = " << tests[i].output << std::endl;
        std::cout << "Answer: " << answer << std::endl;
        return 0;
      }
    } catch(const std::logic_error err) {
      isOk = err.what() == tests[i].output;

      if (isOk) {
        std::cout << "Test " << i + 1 << " passed" << std::endl;
      } else {
        std::cout << "Test " << i + 1 << " failed" << std::endl;
        std::cout << "Expression: " << tests[i].input << " should call '" << tests[i].output <<  "'" << std::endl;
        return 0;
      }
    }
  }
}