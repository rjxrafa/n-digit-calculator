//
// Created by rafab on 5/29/2019.
//

#include "../includes/io.h"

/**
 * todo : this function needs input validation
 * @param input
 * @return
 */
bool GetInput(std::string &input, std::string &&question) {

  fflush(stdin);
  printf("%s\n", question.c_str());
  getline(std::cin, input);

  if (input.find_first_not_of("1234567890+- /") < input.size()) {
    std::cout << "Invalid input! Please try again!\n";
    return false;
  } else {
    return true;
  }


}

/**
 * This function will take a given input and return an equivalent string with comma separated digits.
 * @param input
 * @return
 */
std::string CommaSeparator(const std::string &input) {
  std::string result;
  bool negative = false;

  if (input[0] == '-')
    negative = true;

  unsigned int count = 0;
  for (auto i = input.rbegin(); i != input.rend(); ++i, ++count) {
    if ((count % 3) == 0)
      result = ',' + result;

    result = (*i + result);
  }

  while (result[0] == ',' || result[0] == '0' || result[0] == '-')
    result.erase(0,1);

  if (result[result.size()-1] == ',')
    result.erase(result.size()-1);

  if (negative)
    result.insert(result.begin(), '-');

  return result;
}



