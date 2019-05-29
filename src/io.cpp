//
// Created by rafab on 5/29/2019.
//

#include "../includes/io.h"

/**
 * todo : this function needs input validation
 * @param input
 * @return
 */
bool GetInput(std::string &input) {


  return false;
}

/**
 * This function parses a given string and inserts all characters into a vector.
 * @param input
 * @return vector<char>
 */
std::vector<int> ParseString(std::string &input) {

  std::vector<int> number;
  for (auto &x : input) {
    number.push_back(x-'0');
  }

  return number;
}

/**
 * This function will output to a given output stream a given vector
 * @param output
 * @param out
 */
void Output(std::vector<int> &output, std::ostream &out) {
  for (auto &x : output) {
    out << x;
  }
}


