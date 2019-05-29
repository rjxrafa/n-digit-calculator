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

  auto sum_pos = sum.rbegin();

  uint8_t temp = carry;
  carry = 0;

  for (auto it = op2.rbegin(); it != op2.rend(); ++it, ++) {
    // Starting with the lowest digit of op1 and op2
    if (temp > 9) {
      ++carry;
      temp -= 10;
    }
      *sum_pos = temp+'0'; // Add to the sum
  }
//
  while (carry > 0) {

  }
  if (sum_pos == sum.rend()) {
    sum.insert(sum.begin(), carry + '0');
  } else
    *(++sum_pos) = carry + '0';

  return sum;

}
