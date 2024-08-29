#pragma once

#include "Token.hpp"
#include <vector>

namespace prs {
namespace math {

enum class CharEnum {
  Blank,
  Number,
  Symbol,
  Word,
};
enum class TokenEnum {
  Base = 0, // base should always be 0
  Literal,
  Identifier,
};

std::vector<Token<TokenEnum>> tokenize(const std::string &exprstr);

} // namespace math
} // namespace prs
