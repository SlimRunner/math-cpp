#include "Exceptions.hpp"
#include "Interfaces.hpp"
#include "Token.hpp"
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <variant>
#include <algorithm>

class Foo : public Arithmetic<int, Foo>, public Divisible<int, Foo> {
private:
  int a;

public:
  Foo(int v) : a{v} {}
  int operator+(Foo rhs) { return a + rhs.a; }
  int operator-(Foo rhs) { return a - rhs.a; }
  int operator-() { return -a; }
  int operator*(Foo rhs) { return a * rhs.a; }
  int operator/(Foo rhs) { return a / rhs.a; }
};

std::ostream &operator<<(std::ostream &ostr, prs::TokenType rhs) {
  using namespace prs;
  switch (rhs) {
  case TokenType::Literal:
    ostr << "Literal";
    break;
  case TokenType::Operator:
    ostr << "Operator";
    break;
  case TokenType::Identifier:
    ostr << "Identifier";
    break;
  default:
    break;
  }
  return ostr;
}

int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;

  // auto ptkstr = " (5 sin(3.1456) +  5 9)*3+5*3+55-0.32-.23+.-4 <= 5 coeff ";
  auto ptkstr = "a 5b c d e f5f _ w % + - * /  < > = <= >= )( . 2 2.3";
  try {
    auto ptk = prs::tokenize(ptkstr);

    std::size_t colsize = 0;    
    for (auto const &t : ptk) {
      colsize = std::max(colsize, t.payload().size() + 2);
    }

    std::cout << ptkstr << "\n";
    for (auto const &t : ptk) {
      std::cout << std::setw(colsize + 1) << std::left;
      std::cout << "'" + t.payload() + "'" ;
      std::cout << t.type() << "\n";
    }
    std::cout << "\n";
  } catch (const err::parse_error &e) {
    std::cerr << e.what() << '\n';
    std::cerr << ptkstr << '\n';
    std::cerr << std::setw(e.at() + 1) << '^' << '\n';
    return 1;
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  return 0;
}
