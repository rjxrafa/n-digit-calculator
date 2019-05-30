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

  if (input.find_first_not_of("1234567890+-") < input.size()) {
    std::cout << "Invalid input! Please try again!\n";
    return false;
  } else {
    return true;
  }


}



