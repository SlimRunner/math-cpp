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
  Operator,
};

using TokenList = std::vector<Token<TokenEnum>>;

TokenList tokenize(const std::string &exprstr);

} // namespace math
} // namespace prs
