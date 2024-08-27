#include "Interfaces.hpp"
#include "Token.hpp"
#include <iostream>
#include <string>
#include <string_view>
#include <variant>

class Foo: public Arithmetic<int, Foo>, public Divisible<int, Foo> {
  private:
  int a;
  public:
  Foo(int v) : a{v} { }
  int operator+(Foo rhs) {
    return a + rhs.a;
  }
  int operator-(Foo rhs) {
    return a - rhs.a;
  }
  int operator-() {
    return -a;
  }
  int operator*(Foo rhs) {
    return a * rhs.a;
  }
  int operator/(Foo rhs) {
    return a / rhs.a;
  }
};

int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;

  auto ptkstr = "(sin(3.1456) + 5 9)*3+5*3+55-0.32-.23+.-4 <= 5";
  auto ptk = prs::tokenize(ptkstr);
  std::cout << ptkstr << "\n";
  for (auto const & t: ptk) {
    std::cout << t.payload() << " ";
  }
  std::cout << "\n";

  return 0;
}
