#include "../includes/math.h"

/**
 * This function with add two strings together.
 * @param op1
 * @param op2
 * @return
 */
std::string Add(std::string op1, std::string op2) {
  bool negative = false, fraction = false;

  if (op1.find('/') != std::string::npos || op2.find('/') != std::string::npos) {
    NormalizeFractions(op1, op2); // This submethod turns all operands to equivalent fractions.
    fraction = true;
  }

  if (op1.empty()) { // This routine will check for empty strings or 0
    if (op2.empty())
      return "0";
    else
      return op2;
  } else if (op2.empty()) {
    return op1;
  }

  if (op1[0] == '-') { // This routine will check for negatives // todo:fractions
    if (op2[0] == '-') {
      negative = true;
      op1 = op1.substr(1);
      op2 = op2.substr(1);
    } else { // op1 (-), op2 (+)
      op1 = op1.substr(1);
      return Subtract(op2, op1);
    }
  } else if (op2[0] == '-') { // op1 (+), op2 (-)
    op2 = op2.substr(1);
    return Subtract(op1, op2);
  }

  std::string denominator = "1";

  if (fraction) {
    std::stringstream ss(op1);
    getline(ss, op1, '/');
    ss >> denominator;
    ss.clear();
    ss.str(op2);
    getline(ss, op2, '/');
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

  return (denominator == "1") ? sum : Divide(sum, denominator);
}

/**
 * This function subtracts the first operand from second operand.
 * @param op1
 * @param op2
 * @return
 */
std::string Subtract(std::string op1, std::string op2) {
  // todo check for negatives
  bool negative = false;

  if (op1.empty())
    op1 = "0";
  if (op2.empty())
    op2 = "0";

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
 * This is a helper function to determine which of two operands are smaller.
 * @param op1
 * @param op2
 * @return
 */
bool IsSmaller(const std::string &op1, const std::string &op2) {

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
std::string Multiply(std::string op1, std::string op2) {

  if (op1.empty() || op1 == "0" || op2.empty() || op2 == "0")
    return "0";

  bool negative = false;

  if (op1[0] == '-') {
    negative = !negative;
    op1 = op1.substr(1);
  }

  if (op2[0] == '-') {
    negative = !negative;
    op2 = op2.substr(1);
  }

  if (op1 == "1")
    return ((negative) ? "-":"") + op2;
  else if (op2 == "1")
    return ((negative) ? "-":"") + op1;

  if (op1.size() < op2.size()) {
    std::swap(op1,op2);
  }

  std::string product = "0", temp_product;
  int carry = 0, place = 0;

  for (auto i = op1.rbegin(); i != op1.rend(); ++i, ++place) {
    temp_product = "";
    for (auto j = op2.rbegin(); j != op2.rend(); ++j) {
      int temp = (*i-'0') * (*j-'0') + carry;
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

    for (int k = 0; k < place; ++k) {
      temp_product += '0';
    }

    product = Add(temp_product, product);
  }

  if (negative)
    product = '-' + product;

  return product.empty() ? "0" : product;
}

/**
 * This function computes a given number's factorial.
 * @param op
 * @param threaded
 * @return
 */
std::string Factorial(std::string &op, const bool &&threaded){ // todo : add threading operation

  if (op == "0")
    return "1";

  // Check for negatives and fractions
  if (op[0] == '-' || op.find('/') != std::string::npos) {
    printf("Invalid input!");
    return op;
  }

  // Remove leading zeros
  while (op[0] == '0')
    op = op.substr(1);

  std::string factorial = "1",
      temp = "1",
      increment = "1";

  while (temp != op) {
    temp = Add(temp, increment);
    factorial = Multiply(factorial, temp);
  }

  return factorial;
}

/**
 * This function performs division between two operands. This function uses the restoring division algorithm
 * which can be found here:
 *  https://web.stanford.edu/class/ee486/doc/chap5.pdf
 *
 * @param op1 dividend
 * @param op2 divisor
 * @param mod if true, returns the result of the remainder
 * @return quotient
 */
std::string Divide(std::string op1, std::string op2, const bool &&mod) {

  bool negative = false;

  if (op1.empty())
    op1 = "0";
  if (op2.empty())
    op2 = "0";

  if (op1[0] == '-') {
    negative = !negative;
    op1 = op1.substr(1);
  }

  if (op2[0] == '-') {
    negative = !negative;
    op2 = op2.substr(1);
  }

  while (op1[0] == '0')
    op1 = op1.substr(1);

  while (op2[0] == '0')
    op2 = op2.substr(1);

  if (op2.empty()) {
    printf("Divison by zero! Error.\n");
    return "NaN";
  }

  if (op1 == op2)
    return (negative) ? "-1" :"1";

  if (IsSmaller(op1, op2)) {
    if (negative)
      op1 = '-'+op1;
    return op1 + "/"+ op2;
  } else {

    /**
     * Let
     * N = numerator
     * D = denominator
     * op1.length()-1 = # of bits
     * R = partial remainder
     * q(i) bit #i of quotient
     */

    std::string remainder = op1;
    std::string quotient(op1.length(), '0');
    int q_i = 1;

    for (int i = op1.length()-1; i >= 0; ) {
      remainder = Subtract(remainder, Multiply(op2, Power("10", std::to_string(i))));

      if (remainder[0] == '-') {
        quotient[i] = (q_i-1)+'0';
        q_i = 1;
        remainder = Add(remainder, Multiply(op2, Power("10", std::to_string(i))));
        --i;
      }
      else
        ++q_i;
    }

    std::reverse(quotient.begin(), quotient.end());
    while (quotient[0] == '0')
      quotient = quotient.substr(1);

    if (remainder != "0")
      quotient.append(' '+remainder+'/'+op2);

    if (mod)
      return remainder;

    return (negative) ? '-'+quotient : quotient;
  }
}

/**
 * This power will take a given
 * @param op1
 * @param op2
 * @return
 */
std::string Power(std::string op1, std::string op2) {
  // todo, negative powers

  if (op2 == "0")
    return "1";

  std::string result = op1;

  while (op2 != "1") {
    op2 = Subtract(op2, "1");
    result = Multiply(result, op1);
  }
  return result;
}

void GenerateDivisors(std::vector<std::string> &v, const std::string &num) {
  std::string it = "1";

  while (it != num) {

    std::string temp = Divide(num, it);
    if (temp.find("/") == std::string::npos) {
      v.push_back(temp);
    }
    it = Add(it, "1");
  }
}

/**
 * This function calculates the GCD for two given operands. This implementation uses the Euclidian algorithm for
 * finding GCD between two numbers.
 * @param op1
 * @param op2
 * @return
 */
std::string GCD(std::string op1, std::string op2) {

  if (op1[0] == '-') {
    op1 = op1.substr(1);
  }

  if (op2[0] == '-') {
    op2 = op2.substr(1);
  }

  while (op1[0] == '0')
    op1 = op1.substr(1);

  while (op2[0] == '0')
    op2 = op2.substr(1);

  if (op2.empty() || op1.empty()) {
    printf("Divison by zero! Error.\n");
    return "NaN";
  }

  if (IsSmaller(op1, op2))
    std::swap(op1, op2);

  std::string remainder = Divide(op1, op2, true);
  std::string a = op2, b = remainder;

  while (remainder != "0") {
    remainder = Divide(a, b, true);
    a = b;
    b = remainder;
  }
  return a;
}

/**
 * This function negates a given operand.
 * @param op
 * @return
 */
std::string Negate(const std::string &op) {

  if (op.empty())
    return "0";

  if (op[0] == '-')
    return op.substr(1);
  else
    return '-'+op;
}

/**
 * This function simplifies a given fraction and returns its most simplified form.
 * This function supports mixed numbers as well.
 * @param op
 * @return
 */
std::string SimplifyFraction(const std::string &op) {

  std::string output = op;
  /** Removing leading zeroes. **/
  while (output[0] == '0')
    output = output.substr(1);

  if (op.empty())
    return "0";

  bool negative = false;

  if (output[0] == '-') {
    output = output.substr(1);
    negative = true;
  }

  std::string numerator, denominator, mixed;
  std::stringstream ss(output);

  /** Check if operand is a mixed number **/
  if (output.find(' ') != std::string::npos) {
    ss >> mixed;
    ss.get(); // Remove space
  }

  /** Check if operand is a fraction **/
  if (output.find('/') != std::string::npos) {
    getline(ss, numerator, '/'); // Retrieve numerator & denominator
    ss >> denominator;
  } else if (!mixed.empty()){
    return mixed;
  }

  numerator = Add(numerator, Multiply(denominator, mixed)); // Add mixed, if applicable

  std::string divisor = GCD(numerator, denominator);

  numerator = Divide(numerator, divisor);
  denominator = Divide(denominator, divisor);

  /** Convert improper fractions into mixed numbers. Takes negative into account. **/
  if (IsSmaller(denominator, numerator)) {
    return (negative) ? '-'+Divide(numerator, denominator) : Divide(numerator, denominator);
  }

  if (negative)
    numerator.insert(0, "-");

  return (numerator + ((denominator == "1") ? "" : "/"+denominator));
}

/**
 * This function will "normalize" the denominators of the given fraction to allow for operations on the numerator.
 * @param op1
 * @param op2
 */
void NormalizeFractions(std::string &op1, std::string &op2) {

  bool op1_is_negative = false, op2_is_negative = false;

  /** Trim negatives and zeroes **/
  if (op1[0] == '-') {
    op1 = op1.substr(1);
    op1_is_negative = true;
  }

  if (op2[0] == '-') {
    op2 = op2.substr(1);
    op2_is_negative = true;
  }

  while (op1[0] == '0') {
    op1 = op1.substr(1);
  }

  while (op2[0] == '0') {
    op2 = op2.substr(1);
  }

  if (op1.empty() || op2.empty())
    return;

  std::string mixed_op1, numerator_op1, denominator_op1;
  std::stringstream ss(op1);

  if (op1.find('/') == std::string::npos) { // If it cant find a fraction portion, assume it is a whole number
    if (op1.empty()) { // Accounts for 0
      numerator_op1 = "0";
    } else
      numerator_op1 = op1;
    denominator_op1 = "1";
  } else {
    /** Take in mixed numbers and turn into fractions **/
    if (op1.find(' ') != std::string::npos) {
      ss >> mixed_op1;
      ss.get();
    }

    getline(ss, numerator_op1, '/');
    getline(ss, denominator_op1);

    if (!mixed_op1.empty()) {
      numerator_op1 = Add(numerator_op1, Multiply(denominator_op1, mixed_op1));
    }
  }

  /** Repeat the same process for operand 2 **/
  std::string mixed_op2, numerator_op2, denominator_op2;
  ss.clear();
  ss.str(op2);

  /** Take in mixed numbers and turn into fractions **/
  if (op2.find('/') == std::string::npos) { // If it cant find a fraction portion, assume it is a whole number
    if (op2.empty()) { // Accounts for 0
      numerator_op2 = "0";
    } else
      numerator_op2 = op2;
    denominator_op2 = "1";

  } else {
    if (op2.find(' ') != std::string::npos) {
      ss >> mixed_op2;
      ss.get();
    }

    getline(ss, numerator_op2, '/');
    getline(ss, denominator_op2);

    if (!mixed_op2.empty())
      numerator_op2 = Add(numerator_op2, Multiply(denominator_op2, mixed_op2));
  }

  op1 = ((op1_is_negative) ? "-" : "") +
        Multiply(numerator_op1, denominator_op2) + "/" + Multiply(denominator_op1, denominator_op2);
  op2 = ((op2_is_negative) ? "-" : "") +
        Multiply(numerator_op2, denominator_op1) + "/" + Multiply(denominator_op2, denominator_op1);

}
