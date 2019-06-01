//
// Created by rafab on 5/29/2019.
//

#include "../includes/arithmetic.h"

/**
 * This function with add two strinsg together.
 * @param op1
 * @param op2
 * @return
 */
std::string Add(std::string &op1, std::string &op2) {
  // todo : check negatives (check first array)
  bool negative = false;

  if (op1[0] == '-') {
    if (op2[0] == '-') {
      negative = true;
      op1 = op1.substr(1);
      op2 = op2.substr(1);
    } else { // op1 (-), op2 (+)
      op1 = op1.substr(1);
      return '-' + Subtract(op1, op2);
    }
  } else if (op2[0] == '-') { // op1 (+), op2 (-)
    op2 = op2.substr(1);
    return Subtract(op1, op2);
  }

  if (op2.size() > op1.size())
    std::swap(op1,op2);

  bool carry = false;
  std::string sum = op1; // Make a copy of the largest number and add the smaller number to it
  int sum_it = sum.size()-1;

  uint8_t temp = carry;

  for (int i = op2.size()-1; i >= 0; --i, --sum_it) {
    // Starting with the lowest digit of op1 and op2
    if (carry) {
      ++sum[sum_it];
      carry = false;
    }

    sum[sum_it] += op2[i]-'0'; // add integer value of op2
    if ((sum[sum_it] - '0') > 9) {
      carry = true;
      sum[sum_it] -= 10;
    }
  }

  while (carry && sum_it >= 0 ) {
    ++sum[sum_it];
    if ((sum[sum_it] - '0') > 9) {
      sum[sum_it] -= 10;
    } else
      carry = false;
    --sum_it;
  }

  if (carry)
    sum = '1'+sum;

  while (sum[0] == '0')
    sum = sum.substr(1);

  if (negative)
    sum = '-'+sum;

  return sum;
}

/**
 * This function subtracts first operand from second operand.
 * @param op1
 * @param op2
 * @return
 */
std::string Subtract(std::string &op1, std::string &op2) {
  // todo check for negatives
  bool negative = false;

  if (op1[0] == '-') { // op1 (-)
    if (op2[0] != '-') { // op1(-), op2 (+)
      op1 = op1.substr(1);
      return '-' + Add(op1, op2);
    } else { // op1(-), op2(-)
      negative = true;
      op1 = op1.substr(1);
      op2 = op2.substr(1);
    }
  } else if (op2[0] == '-') { // op1 (+), op2 (-)
    op2 = op2.substr(1);
    return Add(op1, op2);
  }

  if (IsSmaller(op1,op2)) {
    std::swap(op1, op2);
    negative = !negative;
  }

  std::string diff = op1;  // Make a copy of the largest number and subtract the smaller number from it
  int diff_it = op1.size()-1;

  for (int i = op2.size()-1; i >= 0; --i, --diff_it) {


    if (diff[diff_it] < op2[i]) {
      if (diff[diff_it-1]-'0' != 0) {
        --diff[diff_it-1];
      } else { // borrow 1 from the next digit
        int count = diff_it-1;
        while (diff[count] == '0') {
          diff[count] = '9';
          --count;
        }
        --diff[count];
      }
      diff[diff_it] += (10 - (op2[i]-'0'));
    } else {
      diff[diff_it] -= (op2[i]-'0');
    }

  }


  while (diff[0] == '0')
    diff = diff.substr(1);

  return diff.empty() ? diff+'0' : (negative) ? ('-'+diff) : (diff);
}

/**
 * This is a helper function to determine which of two operands are smaller
 * @param op1
 * @param op2
 * @return
 */
bool IsSmaller(std::string &op1, std::string &op2) {

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

/**
 * This is a function that multiplies two n-digit strings.
 * @param op1
 * @param op2
 * @return
 */
std::string Multiply(std::string &op1, std::string &op2) {

  bool negative = false;

  if (op1[0] == '-') {
    negative = !negative;
    op1 = op1.substr(1);
  }

  if (op2[0] == '-') {
    negative = !negative;
    op2 = op2.substr(1);
  }

  if (op1.size() < op2.size()) {
    std::swap(op1,op2);
  }

  std::reverse(op1.begin(), op1.end());
  std::reverse(op2.begin(), op2.end());

  std::string product = "0", temp_product;
  int carry = 0;
  for (unsigned int i = 0; i < op1.size(); ++i) {
    temp_product = "";
    for (unsigned int j = 0; j < op2.size(); ++j) {
      int temp = (op1[i]-'0') * (op2[j]-'0') + carry;
      carry = 0;
      if (temp > 9) {
        carry = temp/10;
      }
      temp_product = std::to_string(temp%10) + temp_product;
    }

    if (carry) {
      temp_product = std::to_string(carry) + temp_product;
      carry = 0;
    }

    for (int k = 0; k < i; ++k) {
      temp_product += '0';
    }

    product = Add(temp_product, product);
  }

  if (negative)
    product = '-' + product;


  std::reverse(op1.begin(), op1.end());
  std::reverse(op2.begin(), op2.end());

  return product.empty() ? "0" : product;

}

std::string Factorial(std::string &op, const bool &&threaded){

  if (op == "0")
    return "1";

  if (op[0] == '-') {
    printf("Invalid input! No negative numbers allowed.\n");
    return op;
  }

  std::string factorial = "1",
              temp = "1",
              increment = "1";

  while (temp != op) {
    temp = Add(temp, increment);
    factorial = Multiply(factorial, temp);
  }

  return factorial;
}

