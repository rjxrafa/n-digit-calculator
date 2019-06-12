#include "../includes/math.h"

/**
 * This function will add two strings together. Supports mixed numbers and fractions.
 * Mixed numbers and fractions are separated as follows: "3_1|2"
 * @param op1
 * @param op2
 * @return
 */
std::string Add(std::string op1, std::string op2) {

  bool fraction = false,
       negative = false;

  /** Handle negatives **/
  if (op1[0] == '-') { // This routine will check for negatives
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

  while (op1[0] == '0') {
    op1 = op1.substr(1);
  }
  while (op2[0] == '0') {
    op2 = op2.substr(1);
  }

  /** This routine checks for empty strings or 0 **/
  if (op1.empty()) { // This routine will check for empty strings or 0
    if (op2.empty())
      return "0";
    else
      return SimplifyFraction(op2);
  } else if (op2.empty()) {
    return SimplifyFraction(op1);
  }

  /** This submethod turns all operands to their equivalent fraction. **/
  if (op1.find('|') != std::string::npos || op2.find('|') != std::string::npos) {
    fraction = true;
    NormalizeFractions(op1, op2); // This submethod turns all operands to equivalent fractions.
  }

  /** Mixed Num / Fraction parser **/
  std::string op1_mixed = "0", op2_mixed = "0";
  std::string common_denominator = "1";

  /** This method will parse for the numerator and common denominator. **/
  if (fraction) {
    std::stringstream ss(op1);
    if (op1.find('_') != std::string::npos)
      getline(ss, op1_mixed, '_');
    getline(ss, op1, '|');
    getline(ss, common_denominator);

    ss.clear(); ss.str(op2);
    if (op2.find('_') != std::string::npos)
      getline(ss, op2_mixed, '_');
    getline(ss, op2, '|');
    getline(ss, common_denominator);

    if (op1_mixed != "0") // op1 has mixed, add to op1's numerator
      op1 = Add(Multiply(op1_mixed, common_denominator), op1);
    if (op2_mixed != "0") // op2 has mixed, add to op2's numerator
      op2 = Add(Multiply(op1_mixed, common_denominator), op2);
  }

  if (op2.size() > op1.size()) // This routine is for ease in addition. (Adding smaller number to larger num)
    std::swap(op1,op2);

  bool carry = false;
  std::string sum = op1;
  int sum_it = sum.size()-1;

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

  while (sum[0] == '0') // remove leading zeroes
    sum = sum.substr(1);

  if (negative)
    sum = '-'+sum;

  if (fraction) {
    sum = sum + "|" + common_denominator;
  }

  sum = SimplifyFraction(sum);
  return sum;
}

/**
 * This function subtracts the first operand from second operand. Supports mixed numbers and fractions.
 * Mixed numbers and fractions are separated as follows: "3_1|2"
 * @param op1
 * @param op2
 * @return
 */
std::string Subtract(std::string op1, std::string op2) {

  bool fraction = false,
       negative = false;

  /** Handle negatives **/
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

  while (op1[0] == '0') {
    op1 = op1.substr(1);
  }
  while (op2[0] == '0') {
    op2 = op2.substr(1);
  }

  /** This routine checks for empty strings or 0 **/
  if (op1.empty()) { // This routine will check for empty strings or 0
    if (op2.empty())
      return "0";
    else {
      op2 = SimplifyFraction(op2);
      return (negative) ? op2 : ('-'+op2);
    }
  } else if (op2.empty()) {
    op1 = SimplifyFraction(op1);
    return op1;
  }

  /** This submethod turns all operands to their equivalent fraction. **/
  if (op1.find('|') != std::string::npos || op2.find('|') != std::string::npos) {
    fraction = true;
    NormalizeFractions(op1, op2); // This submethod turns all operands to equivalent fractions.
  }

  /** Mixed Num / Fraction parser **/
  std::string op1_mixed = "0", op2_mixed = "0";
  std::string common_denominator = "1";

  /** This method will parse for the numerator and common denominator. **/
  if (fraction) {
    std::stringstream ss(op1);
    if (op1.find('_') != std::string::npos)
      getline(ss, op1_mixed, '_');
    getline(ss, op1, '|');
    getline(ss, common_denominator);

    ss.clear(); ss.str(op2);
    if (op2.find('_') != std::string::npos)
      getline(ss, op2_mixed, '_');
    getline(ss, op2, '|');
    getline(ss, common_denominator);

    if (op1_mixed != "0") // op1 has mixed, add to op1's numerator
      op1 = Add(Multiply(op1_mixed, common_denominator), op1);
    if (op2_mixed != "0") // op2 has mixed, add to op2's numerator
      op2 = Add(Multiply(op1_mixed, common_denominator), op2);
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

  if (fraction) {
    if (diff.empty())
      return "0";
    else
      diff = diff + "|" + common_denominator;
  }
  diff = SimplifyFraction(diff);

  return diff.empty() ? "0" : ((negative) ? ('-'+diff) : (diff));
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

  bool fraction = false,
      negative = false;

  /** Handle negatives **/
  if (op1[0] == '-') {
    negative = !negative;
    op1 = op1.substr(1);
  }

  if (op2[0] == '-') {
    negative = !negative;
    op2 = op2.substr(1);
  }

  if (op1 == "1") { // Identity Property
    op2 = SimplifyFraction(op2);
    return ((negative) ? "-":"") + op2;
  } else if (op2 == "1") {
    op2 = SimplifyFraction(op2);
    return ((negative) ? "-":"") + op1;
  }

  /** Handle zeroes **/
  while (op1[0] == '0') {
    op1 = op1.substr(1);
  }
  while (op2[0] == '0') {
    op2 = op2.substr(1);
  }
  if (op1.empty() || op2.empty())
    return "0";

  /** This submethod turns all operands to their equivalent fraction. **/
  if (op1.find('|') != std::string::npos) { // op1 is a fraction
    if (op2.find('|') == std::string::npos) // op2 is not a fraction
      op2 += "|1";
    /** For multiplication, we don't need to normalize but we do need both to be in fraction form **/
    fraction = true;
  } else if (op2.find('|') != std::string::npos) {
    op1 += "|1";
    fraction = true;
  }

  /** Mixed Num / Fraction parser **/
  std::string op1_mixed = "0",
              op2_mixed = "0",
              op1_denominator= "0",
              op2_denominator= "0",
              common_denominator = "1";

  /** This method will parse for the numerator and common denominator. **/
  if (fraction) {
    std::stringstream ss(op1);
    if (op1.find('_') != std::string::npos)
      getline(ss, op1_mixed, '_');
    getline(ss, op1, '|');
    getline(ss, op1_denominator);

    ss.clear(); ss.str(op2);
    if (op2.find('_') != std::string::npos)
      getline(ss, op2_mixed, '_');
    getline(ss, op2, '|');
    getline(ss, op2_denominator);

    if (op1_mixed != "0") // op1 has mixed, add to op1's numerator
      op1 = Add(Multiply(op1_mixed, op1_denominator), op1);
    if (op2_mixed != "0") // op2 has mixed, add to op2's numerator
      op2 = Add(Multiply(op1_mixed, op2_denominator), op2);

    common_denominator = Multiply(op1_denominator, op2_denominator);
  }

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

  if (fraction) {
    product = product + "|" + common_denominator;
    product = SimplifyFraction(product);
  }

  return product.empty() ? "0" : product;
}

/**
 * This function computes a given number's factorial.
 * @param op
 * @param threaded
 * @return
 */
std::string Factorial(std::string op, std::string end) { // todo : add threading operation

  if (op == "0")
    return "1";

  // Check for negatives and fractions
  if (op[0] == '-' || op.find('|') != std::string::npos || op.find('_') != std::string::npos) {
    printf("Invalid input!\n");
    return "{}";
  }

  // Remove leading zeros
  while (op[0] == '0')
    op = op.substr(1);

  std::string factorial = op,
              counter = op,
              decrement = "1";

  counter = Subtract(counter, decrement);
  while (counter != end) { // Count down to 1 from
    factorial = Multiply(factorial, counter);
    counter = Subtract(counter, decrement);
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

  bool negative = false,
       fraction = false;

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

  if (op1.empty())
    op1 = "0";
  if (op2.empty()) {
    printf("Error! Division by zero.");
    return "NaN";
  }

  if (op1 == op2) /** Identity Property **/
    return (negative) ? "-1" :"1";

  /** This submethod turns all operands to their equivalent fraction. **/
  if (op1.find('|') != std::string::npos) { // op1 is a fraction
    if (op2.find('|') == std::string::npos) // op2 is not a fraction
      op2 += "|1";
    /** For multiplication, we don't need to normalize but we do need both to be in fraction form **/
    fraction = true;
  } else if (op2.find('|') != std::string::npos) {
    op1 += "|1";
    fraction = true;
  }

  if (fraction) {
    /** Mixed Num / Fraction parser **/
    std::string op1_mixed = "0",
        op2_mixed = "0",
        op1_denominator= "0",
        op2_denominator= "0",
        common_denominator = "1";

    /** This method will parse for mixed numbers and the fraction parts **/
    if (fraction) {
      std::stringstream ss(op1);
      if (op1.find('_') != std::string::npos)
        getline(ss, op1_mixed, '_');
      getline(ss, op1, '|');
      getline(ss, op1_denominator);

      ss.clear(); ss.str(op2);
      if (op2.find('_') != std::string::npos)
        getline(ss, op2_mixed, '_');
      getline(ss, op2, '|');
      getline(ss, op2_denominator);

      if (op1_mixed != "0") // op1 has mixed, add to op1's numerator
        op1 = Add(Multiply(op1_mixed, op1_denominator), op1);
      if (op2_mixed != "0") // op2 has mixed, add to op2's numerator
        op2 = Add(Multiply(op1_mixed, op2_denominator), op2);

      if (negative)
        op1 = '-'+op1;

      op1 += "|" + op1_denominator;
      op2 = op2_denominator + "|" + op2; // Reverse the divisor (Reciprocal)

      return Multiply(op1, op2);
    }
  }

  if (IsSmaller(op1, op2)) {
    if (negative)
      op1 = '-'+op1;
    return SimplifyFraction(op1 + "|"+ op2);
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
      quotient.append('_'+remainder+'|'+op2);

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
std::string SimplifyFraction(std::string op) {

  if (op.find('|') == std::string::npos) // If op is not a fraction, return op
    return op;

  bool negative = false;
  /** Handle negatives **/
  if (op[0] == '-') {
    op = op.substr(1);
    negative = true;
  }

  /** Removing leading zeroes. **/
  while (op[0] == '0')
    op = op.substr(1);

  if (op.empty())
    return "0";

  std::string numerator, denominator, mixed;
  std::stringstream ss(op);

  /** Check if operand is a mixed number **/
  if (op.find('_') != std::string::npos) {
    ss >> mixed;
    ss.get(); // Remove space
  }

  getline(ss, numerator, '|'); // Retrieve numerator & denominator
  ss >> denominator;

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

  return (numerator + ((denominator == "1") ? "" : "|"+denominator));
}

/**
 * This function will cross essentially cross-multiply two given operands to allow for addition/subtraction.
 * @param op1
 * @param op2
 */
void NormalizeFractions(std::string &op1, std::string &op2) {

  bool op1_is_negative = false,
       op2_is_negative = false;

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

  std::string op1_mixed,
              op1_numerator,
              op1_denominator;
  std::stringstream ss(op1);

  if (op1.find('|') == std::string::npos) { // Case 1: Op1 is not a fraction/mixed number
    if (op1.empty() || op1 == "0") { // Case 1.1: Op1 is zero/empty
      op1_numerator = "0";
    } else // Case 1.2: Op1 is a whole number
      op1_numerator = op1;
    op1_denominator = "1"; // Return 'op1/1'
  } else { // Case 2: Op1 is a fraction/mixed number
    if (op1.find('_') != std::string::npos) { // Case 2.1: op1 is a mixed number
      getline(ss, op1_mixed, '_');
    }
    getline(ss, op1_numerator, '|');
    getline(ss, op1_denominator);

    if (!op1_mixed.empty()) {
      op1_numerator = Add(Multiply(op1_denominator, op1_mixed), op1_numerator);
    }
  }

  /** Repeat the same process for operand 2 **/
  std::string op2_mixed,
              op2_numerator,
              op2_denominator;
  ss.clear(); ss.str(op2);

  if (op2.find('|') == std::string::npos) { // Case 1: Op1 is not a fraction/mixed number
    if (op2.empty() || op2 == "0") { // Case 1.1: Op1 is zero/empty
      op2_numerator = "0";
    } else // Case 1.2: Op1 is a whole number
      op2_numerator = op2;
    op2_denominator = "1"; // Return 'op2/1'
  } else { // Case 2: Op1 is a fraction/mixed number
    if (op2.find('_') != std::string::npos) { // Case 2.1: op2 is a mixed number
      getline(ss, op2_mixed, '_');
    }
    getline(ss, op2_numerator, '|');
    getline(ss, op2_denominator);

    if (!op2_mixed.empty()) {
      op2_numerator = Add(Multiply(op2_denominator, op2_mixed), op2_numerator);
    }
  }

  op1 = ((op1_is_negative) ? "-" : "") +
        Multiply(op1_numerator, op2_denominator) + "|" + Multiply(op1_denominator, op2_denominator);
  op2 = ((op2_is_negative) ? "-" : "") +
        Multiply(op2_numerator, op1_denominator) + "|" + Multiply(op2_denominator, op1_denominator);
}

std::string Combination(std::string op1, std::string op2) {

  if (op1.find('-') != std::string::npos ||
      op1.find('|') != std::string::npos ||
      op1.find('_') != std::string::npos) {
    printf("Error! Invalid input.\n");
    return "{}";
  }

  if (op2.find('-') != std::string::npos ||
      op2.find('|') != std::string::npos ||
      op2.find('_') != std::string::npos) {
    printf("Error! Invalid input.\n");
    return "{}";
  }

  // Remove leading zeros
  while (op1[0] == '0')
    op1 = op1.substr(1);
  while (op2[0] == '0')
    op2 = op2.substr(1);

  if (IsSmaller(op1, op2)) {
    printf("Error! First entry must be larger.\n");
    return "{}";
  }

  std::string numerator,
              denominator;

  numerator = (Factorial(op1));
  denominator = (Multiply(Factorial(op2), Factorial(Subtract(op1, op2))));

  return SimplifyFraction(numerator+"|"+denominator);
}

std::string Permutation(std::string op1, std::string op2) {

  if (op1.find('-') != std::string::npos ||
      op1.find('|') != std::string::npos ||
      op1.find('_') != std::string::npos) {
    printf("Error! Invalid input.\n");
    return "{}";
  }

  if (op2.find('-') != std::string::npos ||
      op2.find('|') != std::string::npos ||
      op2.find('_') != std::string::npos) {
    printf("Error! Invalid input.\n");
    return "{}";
  }

  // Remove leading zeros
  while (op1[0] == '0')
    op1 = op1.substr(1);
  while (op2[0] == '0')
    op2 = op2.substr(1);

  if (IsSmaller(op1, op2))
  {
    printf("Error! Invalid input.\n");
    return "{}";
  }

  return SimplifyFraction((Factorial(op1, Subtract(op1, op2))));
}

bool EqualTo(std::string op1, std::string op2) {

  NormalizeFractions(op1,op2);

  int i;

  for(i = 0; i < op1.size(); ++i)
    if(op1[i] != op2[i])
      return false;

  return i == op2.size();
}

bool LessThan(std::string op1, std::string op2) {

  NormalizeFractions(op1,op2);
  /** Handle negatives **/
  bool negative = false; // If both are negative, invert results
  if (op1[0] == '-') { // If op1 is negative and op2 is not negative, return true
    if (op2[0] != '-') {
      return true;
    }
    op1 = op1.substr(0); // Trim negatives for next step
    op2 = op2.substr(0);
    negative = true;
  } else if (op2[0] == '-') // if op2 is negative and op1 is positive, return false
    return false;

  if (op1 == op2)
    return false;

  std::string op1_numerator, op2_numerator;
  std::stringstream ss(op1);
  getline(ss, op1_numerator, '|');
  ss.clear();
  ss.str(op2);
  getline(ss, op2_numerator, '|');

  return (negative) ? (!IsSmaller(op1_numerator, op2_numerator)) :
                      (IsSmaller(op1_numerator, op2_numerator));
}

bool GreaterThan(std::string op1, std::string op2)
{
  NormalizeFractions(op1,op2);
  /** Handle negatives **/
  bool negative = false; // If both are negative, invert results
  if (op1[0] == '-') { // If op1 is negative and op2 is not negative, return false
    if (op2[0] != '-') {
      return false;
    }
    op1 = op1.substr(0); // Trim negatives for next step
    op2 = op2.substr(0);
    negative = true;
  } else if (op2[0] == '-') // if op2 is negative and op1 is positive, return false
    return true;

  if (op1 == op2)
    return false;

  std::string op1_numerator, op2_numerator;
  std::stringstream ss(op1);
  getline(ss, op1_numerator, '|');
  ss.clear();
  ss.str(op2);
  getline(ss, op2_numerator, '|');

  return (negative) ? (IsSmaller(op1_numerator, op2_numerator)) :
         (!IsSmaller(op1_numerator, op2_numerator));
}
