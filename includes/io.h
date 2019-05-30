//
// Created by rafab on 5/29/2019.
//

#ifndef N_DIGIT_CALCULATOR__INPUT_H_
#define N_DIGIT_CALCULATOR__INPUT_H_

#include "mylib.h"

bool GetInput(std::string &input, std::string &&question);
std::vector<int> StringToVector(const std::string &input);
std::vector<int> StringToVector(const char *cstr);
void Output(const std::vector<int> &output, std::ostream &out);

#endif //N_DIGIT_CALCULATOR__INPUT_H_
