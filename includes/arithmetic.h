//
// Created by rafab on 5/29/2019.
//

#ifndef N_DIGIT_CALCULATOR_SRC_ADD_H_
#define N_DIGIT_CALCULATOR_SRC_ADD_H_

#include "mylib.h"

std::vector<int> Add(std::vector<int> &op1, std::vector<int> &op2);
std::vector<int> Subtract(const std::string &&op1, const std::string &&op2);
bool IsSmaller(std::vector<int> &op1, std::vector<int> &op2);

#endif //N_DIGIT_CALCULATOR_SRC_ADD_H_