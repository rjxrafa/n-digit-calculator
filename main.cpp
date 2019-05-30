#include "includes/io.h"
#include "includes/add.h"

int main() {

  std::string op1, op2;

  while (!GetInput(op1, "Enter operand 1: "));
  while (!GetInput(op2, "Enter operand 2: "));

  std::vector<int> num1 = ParseString(op1);
  std::vector<int> num2 = ParseString(op2);
  std::vector<int> sum = Add(num1, num2);

  Output(sum, std::cout);

  return 0;
}

