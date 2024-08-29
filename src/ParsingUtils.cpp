#include "ParsingUtils.hpp"

#include "Exceptions.hpp"
#include <optional>
#include <cstddef> // size_t
#include <sstream> // size_t

namespace prs {
namespace math {

using CharSet = std::set<SimplType<CharEnum>, SimplType<CharEnum>::TypeSort>;
using CharMap = std::map<char, CharEnum>;
using MathState = TokenState<TokenEnum, CharEnum>;
using MathRule = TokenRule<TokenEnum, CharEnum>;
using StateSet = std::map<TokenEnum, MathState, MathState::TypeSort>;

static void genCharSet(CharMap &lut);
static TokenEnum genStates(StateSet &stateSet);

std::vector<Token<TokenEnum>> tokenize(const std::string &exprstr) {
  CharMap lut;
  StateSet stateSet;

  genCharSet(lut);
  TokenEnum curr = genStates(stateSet);

  std::vector<Token<TokenEnum>> tokens;
  TokenEnum prev = TokenEnum::Base;

  std::size_t index = 0;
  std::stringstream tkPayload;

  for (auto const &c : exprstr) {
    // std::cout << c << ": ";
    if (auto it = lut.find(c); it != lut.cend()) {
      bool isValid = false, isBaseCase = false;
      bool hasChanged = false;

      for (auto && rule : stateSet.at(curr)) {
        auto newState = rule(it->second, c);
        if (newState.has_value()) {
          curr = newState.value();
          isValid = true;
          isBaseCase = curr == TokenEnum::Base;
          hasChanged = prev != curr;
          if (hasChanged) {
            stateSet.at(curr).clearRuleCounts();
          }
          break;
        }
      }
      
      if (!isValid) {
        throw err::unexpected_symbol(index, c);
      }
      if (auto str = tkPayload.str(); hasChanged && str.size() > 0) {
        tokens.push_back({str, prev, index - str.size()});
        tkPayload.str("");
      }
      if (!isBaseCase) {
        tkPayload << c;
      }

      prev = curr;
    } else {
      throw err::invalid_symbol(index, c);
    }

    ++index;
  }
  if (auto tail = tkPayload.str(); tail.size() != 0) {
    tokens.push_back({tkPayload.str(), prev, index - tail.size()});
  }
  return tokens;
}

static void genCharSet(CharMap &lut) {
  CharSet cset = {
    {CharEnum::Blank, {" \t\n\f\v"}},
    {CharEnum::Number, {"1234567890"}},
    {CharEnum::Word, {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}},
    {CharEnum::Symbol, {"_."}},
    // {CharSet::Symbol, {"+-*/_."}},
  };

  for (const auto & cs : cset) {
    for (auto const & c: cs) {
      lut.insert({c, cs.type()});
    }
  }
}

static TokenEnum genStates(StateSet & stateSet) {
  MathState tsBase(TokenEnum::Base);
  tsBase.addRule(MathRule(TokenEnum::Base, [](CharEnum t, char) {
    return t == CharEnum::Blank;
  }));
  tsBase.addRule(MathRule(TokenEnum::Literal, [](CharEnum t, char) {
    return t == CharEnum::Number;
  }));
  tsBase.addRule(MathRule(TokenEnum::Identifier, [](CharEnum t, char s) {
    return t == CharEnum::Word || s == '_';
  }));

  MathState tsLiteral(TokenEnum::Literal);
  tsLiteral.addRule(MathRule(TokenEnum::Base, [](CharEnum t, char) {
    return t == CharEnum::Blank;
  }));
  tsLiteral.addRule(MathRule(TokenEnum::Literal, [](CharEnum t, char) {
    return t == CharEnum::Number;
  }));
  tsLiteral.addRule(MathRule(TokenEnum::Literal, [](CharEnum, char s) {
    return s == '.';
  }, 1));
  tsLiteral.addRule(MathRule(TokenEnum::Identifier, [](CharEnum t, char s) {
    return t == CharEnum::Word || s == '_';
  }));

  MathState tsIdent(TokenEnum::Identifier);
  tsIdent.addRule(MathRule(TokenEnum::Base, [](CharEnum t, char) {
    return t == CharEnum::Blank;
  }));
  tsIdent.addRule(MathRule(TokenEnum::Identifier, [](CharEnum t, char s) {
    return t == CharEnum::Word || t == CharEnum::Number || s == '_';
  }));

  stateSet.insert({TokenEnum::Base, tsBase});
  stateSet.insert({TokenEnum::Literal, tsLiteral});
  stateSet.insert({TokenEnum::Identifier, tsIdent});

  return TokenEnum::Base;
}

} // namespace math
} // namespace prs
