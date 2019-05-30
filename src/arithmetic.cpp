//
// Created by rafab on 5/29/2019.
//

#include "../includes/arithmetic.h"

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

std::vector<int> Subtract(const std::string &op1, const std::string &op2) {
  // todo check for negatives
  bool negative = false;

  // If both strings are negative, then we need to
//  if (IsSmaller(op1, op2)) {
//    std::swap(op1,op2);
//    negative = true;
//  }

  // op1 is bigger than op2

  std::vector<int> diff = op1;  // Make a copy of the largest number and subtract the smaller number from it
  auto diff_pos = op1.rbegin();

  for (auto it = op2.rbegin(); it != op2.rend(); ++it, ++diff_pos) {

    if (*it > *diff_pos) {
      *diff_pos += 10 - *it;
      *(diff_pos+1) -= 1;
    }
  }

  return diff;
}

bool IsSmaller(std::vector<int> &op1, std::vector<int> &op2) {

  if (op1.size() < op2.size())
    return true;
  if (op1.size() > op2.size())
    return false;

  for (int i = 0; i < op1.size(); ++i) {
    if (op1[i] < op2[i])
      return true;
    else if (op1[i] > op2[i])
      return false;
  }

  return false;

}
