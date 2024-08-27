#include "Token.hpp"

#include "Exceptions.hpp"
#include <cctype>  // isalpha,...
#include <cstddef> // size_t
#include <exception>
#include <optional>
#include <sstream>

namespace prs {

enum class SymType {
  None,
  Blank,
  Alpha,
  Digit,
  DDot,
  Symbol,
  Other,
};

static std::size_t countChars(const std::string &str, char match) {
  std::size_t count = 0;
  for (auto const &chr : str) {
    count += chr == match;
  }
  return count;
}

static bool isSymbol(char sym) {
  switch (sym) {
  case '+':
  case '-':
  case '*':
  case '/':
  case '=':
  case '<':
  case '>':
  case '%':
  case ',':
  case '(':
  case ')':
    return true;
  default:
    return false;
  }
}

template <class T> static int computeBFV(T enumVal) {
  return 1 << static_cast<int>(enumVal);
}

static SymType classifySymbol(char chr) {
  if (isalpha(chr) || chr == '_') {
    return SymType::Alpha;
  } else if (isdigit(chr)) {
    return SymType::Digit;
  } else if (chr == '.') {
    return SymType::DDot;
  } else if (isspace(chr)) {
    return SymType::Blank;
  } else if (isSymbol(chr)) {
    return SymType::Symbol;
  } else {
    return SymType::Other;
  }
}

static bool filterSymbol(SymType sym, int filter) {
  return !!(computeBFV(sym) & filter);
}

static std::optional<TokenType> classifyToken(char chr) {
  SymType type = classifySymbol(chr);

  switch (type) {
  case SymType::Alpha:
    return TokenType::Identifier;
  case SymType::Digit:
  case SymType::DDot:
    return TokenType::Literal;
  case SymType::Symbol:
    return TokenType::Operator;
  case SymType::None:
  case SymType::Blank:
  case SymType::Other:
  default:
    return std::nullopt;
  }
}

static int getWhiteList(SymType currSym, char chr) {
  switch (currSym) {
  case SymType::Alpha:
    return computeBFV(SymType::Alpha) | computeBFV(SymType::Digit);
  case SymType::Digit:
    return computeBFV(SymType::Digit) | computeBFV(SymType::DDot);
  case SymType::DDot:
    return computeBFV(SymType::Digit);
  case SymType::Symbol:
    return (chr == '<' || chr == '>' ? computeBFV(SymType::Symbol) : 0);
  case SymType::None:
  case SymType::Blank:
  case SymType::Other:
  default:
    // makes it impossible to match
    return 0;
  }
}

static int getBlackList(SymType currSym, char chr) {
  (void)chr;
  switch (currSym) {
  case SymType::DDot:
    return -1 ^ computeBFV(SymType::Digit);
  case SymType::Digit:
    // return computeBFV(SymType::Alpha);
  case SymType::Symbol:
  case SymType::Alpha:
  case SymType::Blank:
  case SymType::None:
  case SymType::Other:
  default:
    // makes it impossible to match
    return 0;
  }
}

Token::Token(std::string data, TokenType type)
    : m_payload{data}, m_type{type} {}

std::string Token::payload() const { return m_payload; }

TokenType Token::type() const { return m_type; }

std::vector<Token> tokenize(const std::string &exprstr) {
  if (exprstr.size() == 0) {
    return {};
  }
  std::vector<Token> output;
  SymType currSym = classifySymbol(exprstr.front());
  // used to determine if the token can continue to grow
  int whiteList = -1;
  // used to determine if the token is ill formed
  int blackList = 0;
  std::size_t start = 0;

  while (currSym == SymType::Blank && ++start < exprstr.size()) {
    currSym = classifySymbol(exprstr.at(start));
  }

  for (std::size_t i = start; i < exprstr.size(); ++i) {
    char chr = exprstr.at(i);
    currSym = classifySymbol(chr);

    // exception for <= and >=
    if (currSym == SymType::Symbol &&
        (i - start > 1 || (chr != '=' && i - start > 0))) {
      whiteList = 0;
    }

    // exception for .
    if (exprstr.at(start) == '.' && currSym == SymType::Digit &&
        i - start == 1) {
      whiteList = getWhiteList(currSym, chr);
      blackList = getBlackList(currSym, chr);
    }

    // not a valid symbol
    if (currSym == SymType::Other) {
      throw err::invalid_symbol(i, chr);
    }

    if (filterSymbol(currSym, blackList)) {
      throw err::unexpected_symbol(i, (i > 0 ? exprstr.at(i - 1) : '\0'));
    } else if (!filterSymbol(currSym, whiteList)) {
      const auto ttopt = classifyToken(exprstr.at(start));

      if (ttopt.has_value()) {
        output.push_back({exprstr.substr(start, i - start), ttopt.value()});
      } else if (currSym != SymType::Blank) {
        throw err::parse_error(start);
      }

      if (ttopt.value() == TokenType::Literal &&
          countChars(output.back().payload(), '.') > 1) {
        throw err::unexpected_symbol(start, exprstr.at(start));
      }

      while (currSym == SymType::Blank && ++i < exprstr.size()) {
        chr = exprstr.at(i);
        currSym = classifySymbol(chr);
      }

      if (currSym == SymType::Other) {
        throw err::invalid_symbol(i, chr);
      }

      start = i;
      whiteList = getWhiteList(currSym, chr);
      blackList = getBlackList(currSym, chr);
    } else if (start == i) {
      whiteList = getWhiteList(currSym, chr);
      blackList = getBlackList(currSym, chr);
    }
  }

  if (start < exprstr.size()) {
    const auto ttopt = classifyToken(exprstr.at(start));
    if (!ttopt.has_value()) {
      throw err::parse_error(start);
    }

    output.push_back({exprstr.substr(start), ttopt.value()});

    if (ttopt.value() == TokenType::Literal &&
        countChars(output.back().payload(), '.') > 1) {
      throw err::unexpected_symbol(start, exprstr.at(start));
    }
  }

  return output;
}

} // namespace prs
