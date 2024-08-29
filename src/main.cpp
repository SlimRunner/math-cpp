#include "Exceptions.hpp"
#include "ParsingUtils.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>

int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;

  auto str = " (5 sin(3.1456) +  5 9)*3+5*3+55-0.32-0.23+-4 <= 5 coeff ";
  // auto str = "a 5.1b c d e f 5.2f _ w % + - * /  < > = <= >= )( 2 2.3 0.1 0.7";
  // auto str = "1.265 5.2_a5a 456asd12 ";
  // auto str = "1.265+ 56*52+awe+0.* 5.2_a5a 456asd12 ";
  // auto str = "+1 ++-() ----<=<= ** <= 5//-3";
  std::vector<prs::Token<prs::math::TokenEnum>> tokens;
  try {
    tokens = prs::math::tokenize(str);
  } catch (const err::invalid_symbol &e) {
    std::cerr << e.what() << " after '" << e.which() << "'\n";
    std::cerr << str << '\n';
    std::cerr << std::setw(e.at() + 1) << '^' << '\n';
    return 1;
  } catch (const err::unexpected_symbol &e) {
    std::cerr << e.what() << " after '" << e.which() << "'\n";
    std::cerr << str << '\n';
    std::cerr << std::setw(e.at() + 1) << '^' << '\n';
    return 1;
  }

  std::size_t colsize = 0;
  for (auto const &word : tokens) {
    colsize = std::max(colsize, word.payload().size() + 2);
  }

  for (auto const &word : tokens) {
    std::cout << std::setw(colsize + 1) << std::left;
    std::cout << "'" + word.payload() + "'";
    std::cout << static_cast<int>(word.type()) << "\n";
    // std::cout << word.position() << "\n";
  }

  return 0;
}
