//
// Created by rafab on 5/29/2019.
//

#include "../includes/add.h"

std::vector<int> Add(std::vector<int> &op1, std::vector<int> &op2) {
  // todo : check negatives (check first array)
  if (op2.size() > op1.size())
    std::swap(op1,op2);

  unsigned int carry = 0;
  std::vector<int> sum = op1; // Make a copy of the largest number and add the smaller number to it

  auto sum_it = sum.rbegin();

  uint8_t temp = carry;
  carry = 0;

  for (auto it = op2.rbegin(); it != op2.rend(); ++it, ++sum_it) {
    // Starting with the lowest digit of op1 and op2
    *sum_it = *sum_it + *it + carry;
    if (*sum_it > 9) {
      carry = 1;
      *sum_it -= 10;
    }
  }

  while (carry && sum_it != sum.rend()) {
    *sum_it = *sum_it + carry;
    if (*sum_it > 9) {
      carry = 1;
      *sum_it -= 10;
      ++sum_it;
    } else {
      carry = 0;
    }
  }

  if (carry) {
    sum.insert(sum.begin(), carry);
  }

  return sum;

}
