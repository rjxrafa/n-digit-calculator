
#ifndef N_DIGIT_CALCULATOR_SRC_ADD_H_
#define N_DIGIT_CALCULATOR_SRC_ADD_H_

#include "mylib.h"

std::string Add(std::string op1, std::string op2);
std::string Subtract(std::string op1, std::string op2);
std::string Multiply(std::string op1, std::string op2);
std::string Factorial(std::string op, std::string end = "1");
std::string Divide(std::string op1, std::string op2, const bool &&mod = false);
std::string Power(std::string op1, std::string op2);
bool IsSmaller(const std::string &op1, const std::string &op2);
bool IsPrime();
void GenerateDivisors(std::vector<std::string> &v, const std::string &num);
std::string GCD(std::string op1, std::string op2);
std::string Negate(const std::string &op);
std::string SimplifyFraction(std::string op);
void NormalizeFractions(std::string &op1, std::string &op2);
std::string FractionToDecimal(const std::string &op1);
std::string SquareRoot(const std::string &op);
std::string Combination(std::string op1, std::string op2);
std::string Permutation(std::string op1, std::string op2);
bool EqualTo(std::string op1, std::string op2);
bool LessThan(std::string op1, std::string op2);
bool GreaterThan(std::string op1, std::string op2);



#endif //N_DIGIT_CALCULATOR_SRC_ADD_H_
