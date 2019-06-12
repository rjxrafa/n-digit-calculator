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

  if (input.find_first_not_of("~@#$^!1234567890*+-_/ |") < input.size()) {
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
std::string CommaSeparator(std::string input) {
  std::string result, numerator, denominator, mixed;
  bool mixed_num_exists = false;


  if (input.find('|') != std::string::npos) {
    std::stringstream ss(input);
    if (input.find('_') != std::string::npos) {
      mixed_num_exists = true;
      getline(ss, input, '_');
    }
    getline(ss, numerator, '|');
    getline(ss, denominator);
    numerator = CommaSeparator(numerator);
    denominator = CommaSeparator(denominator);
    if (!mixed_num_exists)
      return (numerator + '|' + denominator);
  }

  bool negative = false;

  //if negative set negative to true
  if (input[0] == '-')
    negative = true;

  unsigned int count = 0;
  //add comma every three digits
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

  // adds negative
  if (negative && !result.empty())
    result.insert(result.begin(), '-');

  return (mixed_num_exists) ? (result + '_' + numerator + '|' + denominator) : result;
}



