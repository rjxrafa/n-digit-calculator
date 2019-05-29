#include "includes/io.h"
#include "includes/add.h"

int main() {

  std::string op1 = "123123123123123123123123123112323123918204982983509185901293479012";
  std::string op2 = "1";
  std::vector<int> num1 = ParseString(op1);
  std::vector<int> num2 = ParseString(op2);

//  std::cout << (*num1.end()-1);
//  std::vector<char> sum = Add(num1, num2);

  Output(num1, std::cout);

  return 0;
}

