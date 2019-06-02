#include "includes/io.h"
#include "includes/math.h"

int main() {

  std::string op1, op2, prod;

  while (1) {
    while (!GetInput(op1, "Enter operand 1: "));
    while (!GetInput(op2, "Enter operand 2: "));

    std::cout << GCD(op1,op2);
  }

  return 0;
}

