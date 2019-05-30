#include "includes/io.h"
#include "includes/multiply.h"

int main() {

  std::string op1, op2;

  while (!GetInput(op1, "Enter operand 1: "));
  while (!GetInput(op2, "Enter operand 2: "));

  std::vector<int> num1 = StringToVector(op1);
  std::vector<int> num2 = StringToVector(op2);
  std::vector<int> diff = Subtract(num1, num2);

//  printf("%s", IsSmaller(num1, num2) ? "true" : "false" );
  Output(diff, std::cout);

  return 0;
}

