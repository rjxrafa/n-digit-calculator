#include "includes/io.h"
#include "includes/multiply.h"

int main() {

  std::string op1, op2, sum;

  while (1) {
    while (!GetInput(op1, "Enter operand 1: "));
    while (!GetInput(op2, "Enter operand 2: "));
    sum = Subtract(op1, op2);
    std::cout << sum << '\n';
  }

//  printf("%s", IsSmaller(num1, num2) ? "true" : "false" );


  return 0;
}

