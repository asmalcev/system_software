#include <iostream>
#include <string>

#include "MathInterpreter/MathInterpreter.hpp"

struct test {
  std::string input;
  std::string output;
};

constexpr size_t TEST_START_INDEX = 0;
constexpr size_t TESTS_COUNT = 8;

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
      "(2  * 5)",
      "10"
    },
  };

  bool isOk = false;
  for (size_t i = TEST_START_INDEX; i < TESTS_COUNT; i++) {
    std::string answer = execute_expression(tests[i].input);
    isOk = answer == tests[i].output;
    if (isOk) {
      std::cout << "Test " << i + 1 << " passed" << std::endl;
    } else {
      std::cout << "Test " << i + 1 << " failed" << std::endl;
      std::cout << "Expression: " << tests[i].input << " = " << tests[i].output << std::endl;
      std::cout << "Answer: " << answer << std::endl;
      return 0;
    }
  }
}