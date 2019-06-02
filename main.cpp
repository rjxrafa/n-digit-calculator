#include "includes/io.h"
#include "includes/arithmetic.h"
#include <thread>
#include <afxres.h>

int main() {

  std::string op1, op2, prod;

  while (1) {
    while (!GetInput(op1, "Enter operand 1: "));
    while (!GetInput(op2, "Enter operand 2: "));
//    prod = Multiply(op1, op2);
//    std::cout << prod << '\n';

//    std::cout << Factorial(op1) << '\n';
    std::cout << Divide(op1, op2) << std::endl;


  }

  return 0;
}

