#include <iostream>

#include "PseudoInterpreter/PseudoInterpreter.hpp"

struct test {
  std::string input;
  std::string output;
};

constexpr size_t TEST_START_INDEX = 0;
constexpr size_t TESTS_COUNT = 7;

int main() {
  const test tests[] = {
    {
      "println(2)",
      "Missed ; symbol"
    },
    {
      "if",
      "Operator-if condition must be into rounded brackets and follow behind it"
    },
    {
      "if (2 + 1",
      "Operator-if condition must be into rounded brackets and follow behind it"
    },
    {
      "if (!4 - 0)",
      "Operator-if statement missed"
    },
    {
      "if (!4 - 0) {",
      "Operator-if statement must be into figured brackets and follow behind condition"
    },
    {
      "if (52) {} else",
      "Operator-else statement must be into figured brackets and follow behind condition"
    },
    {
      "if (!52) {} else {",
      "Operator-else statement must be into figured brackets and follow behind condition"
    },
  };

  bool isOk;
  for (size_t i = TEST_START_INDEX; i < TESTS_COUNT; i++) {
    try {
      interpreter_code(clear_input(tests[i].input));
    } catch(const std::logic_error err) {
      isOk = err.what() == tests[i].output;

      if (isOk) {
        std::cout << "Test " << i + 1 << " passed" << std::endl;
      } else {
        std::cout << "Test " << i + 1 << " failed" << std::endl
                  << "Expression: " << tests[i].input << " should call '" << tests[i].output <<  "'" << std::endl
                  << "But called: " << err.what() << std::endl;
        return 0;
      }
    }
  }
}