#include "includes/io.h"
#include "includes/arithmetic.h"
#include <thread>

int main() {

  std::string op1, op2, prod;

  while (1) {
    while (!GetInput(op1, "Enter operand 1: "));
//    while (!GetInput(op2, "Enter operand 2: "));
//    prod = Multiply(op1, op2);
//    std::cout << prod << '\n';

    std::cout << CommaSeparator(Factorial(op1)) << std::endl;
  }

  return 0;
}

