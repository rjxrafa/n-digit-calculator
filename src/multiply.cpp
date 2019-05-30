//
// Created by rafab on 5/29/2019.
//

#include "../includes/multiply.h"

/**
 * This implementation of multiplication uses Karatsuba's algorithm.
 * @param op1
 * @param op2
 * @return
 */
std::vector<int> Multiply(std::vector<int> &op1, std::vector<int> &op2) {
  while (op1.size() != op2.size()) {
    if (op1.size() < op2.size())
      op1.insert(op1.begin(), 0);
    else
      op2.insert(op2.begin(), 0);
  }

  int m = op1.size(),
      m2 = m >> 1;

  std::vector<int> op1_1(op1.begin(), op1.begin() + op1.size()/2),
                   op1_2(op1.begin() + op1.size()/2 , op1.end());

  std::vector<int> op2_1(op2.begin(), op2.begin() + op2.size()/2),
                   op2_2(op2.begin() + op2.size()/2 , op2.end());

  if (op1.size() == 1) {
    StringToVector(std::to_string(op1.at(1) * op1.at(1)));
  } else {
    auto z0 = Multiply(op1_1, op2_1),
         z1 = Multiply(op1_2, op2_2);

    auto z2 = Add(op1_1, op1_2),
         z3 = Add(op2_1, op2_2),
         z4 = Multiply(z2,z3);

//    std::vector<int> product = ;
  }
//    return Add(std::vector<int>op1.at(1) * op2.at(1))

  Output(op1, std::cout);
  std::cout << "\n";
  Output(op2, std::cout);
}
