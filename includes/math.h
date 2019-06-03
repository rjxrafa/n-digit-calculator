//
// Created by rafab on 5/29/2019.
//

#ifndef N_DIGIT_CALCULATOR_SRC_ADD_H_
#define N_DIGIT_CALCULATOR_SRC_ADD_H_

#include "mylib.h"

std::string Add(std::string op1, std::string op2);
std::string Subtract(std::string op1, std::string op2);
std::string Multiply(std::string op1, std::string op2);
std::string Factorial(std::string &op, const bool &&threaded = false);
std::string Divide(std::string op1, std::string op2, const bool &&mod = false);
std::string Power(std::string op1, std::string op2);
bool IsSmaller(const std::string &op1, const std::string &op2);
bool IsPrime();
void GenerateDivisors(std::vector<std::string> &v, const std::string &num);
std::string GCD(std::string op1, std::string op2);
std::string Negate(std::string op);

#endif //N_DIGIT_CALCULATOR_SRC_ADD_H_