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
  printf("%s", question.c_str());
  getline(std::cin, input);
  fflush(stdin);

  if (input.find_first_not_of("^!1234567890*+- /") < input.size()) {
    printf("Invalid input!\n");
    return false;
  } else {
      // todo
    return true;
  }
}

/**
 * This function will take a given input and return an equivalent string with comma separated digits.
 * @param input
 * @return
 */
std::string CommaSeparator(const std::string &input) {
  //todo, take fractions into account.
  std::string result;
  bool negative = false;

  //if negative set negative to true
  if (input[0] == '-')
    negative = true;

  unsigned int count = 0;
  //add comma every three spaces
  for (auto i = input.rbegin(); i != input.rend(); ++i, ++count) {
    if ((count % 3) == 0)
      result = ',' + result;

    result = (*i + result);
  }

  //removes leading zeroes
  while (result[0] == ',' || result[0] == '0' || result[0] == '-')
    result.erase(0,1);

  //removes trailing comma
  if (result[result.size()-1] == ',')
    result.erase(result.size()-1);

  //adds negative
  if (negative)
    result.insert(result.begin(), '-');

  return result;
}



