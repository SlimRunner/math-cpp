#include "Exceptions.hpp"
#include "ParsingUtils.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;
  
  // auto str = " (5 sin(3.1456) +  5 9)*3+5*3+55-0.32-.23+.-4 <= 5 coeff ";
  // auto str = "a 5.1b c d e f5.2f _ w % + - * /  < > = <= >= )( 2 2.3 .1 0. 7";
  std::string str = "1.265 5.2_a5a 456asd12 ";
  auto tokens = prs::math::tokenize(str);

  std::size_t colsize = 0;    
  for (auto const &word : tokens) {
    colsize = std::max(colsize, word.payload().size() + 2);
  }

  for (auto const &word: tokens) {
    std::cout << std::setw(colsize + 1) << std::left;
    std::cout << "'" + word.payload() + "'" ;
    std::cout << static_cast<int>(word.type()) << "\n";
    // std::cout << word.position() << "\n";
  }

  return 0;
}
