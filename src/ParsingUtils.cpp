#include "ParsingUtils.hpp"

#include "Exceptions.hpp"
#include <optional>
#include <cstddef> // size_t
#include <sstream> // ostringstream
#include <utility> // move

namespace prs {
namespace math {

using CharSet = std::set<SimplType<CharEnum>, SimplType<CharEnum>::TypeSort>;
using CharMap = std::map<char, CharEnum>;
using MathState = TokenState<TokenEnum, CharEnum>;
using MathRule = TokenRule<TokenEnum, CharEnum>;
using StateSet = std::map<TokenEnum, MathState, MathState::TypeSort>;
using VecString = std::vector<std::string>;

static void genCharSet(CharMap &lut, VecString &ligatures);
static TokenEnum genStates(StateSet &stateSet);

TokenList tokenize(const std::string &exprstr) {
  CharMap lut;
  StateSet stateSet;
  VecString ligs;

  genCharSet(lut, ligs);
  TokenEnum curr = genStates(stateSet);

  TokenList tokens;
  TokenEnum prev = TokenEnum::Base;

  std::size_t index = 0;
  std::ostringstream tkPayload;

  for (auto const &c : exprstr) {
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

static void genCharSet(CharMap &lut, VecString &ligatures) {
  CharSet cset = {
    {CharEnum::Blank, {" \t\n\f\v"}},
    {CharEnum::Number, {"1234567890"}},
    {CharEnum::Word, {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"}},
    {CharEnum::Symbol, {"+-*/_.<=>,()%"}},
  };

  for (const auto & cs : cset) {
    for (auto const & c: cs) {
      lut.insert({c, cs.type()});
    }
  }
  for (const std::string str : {"<=", ">=", "**", "//"}) {
    ligatures.push_back(str);
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
  tsBase.addRule(MathRule(TokenEnum::Operator, [](CharEnum t, char s) {
    return t == CharEnum::Symbol && s != '_' && s != '.';
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
  tsLiteral.addRule(MathRule(TokenEnum::Operator, [](CharEnum t, char s) {
    return t == CharEnum::Symbol && s != '_' && s != '.';
  }));

  MathState tsIdent(TokenEnum::Identifier);
  tsIdent.addRule(MathRule(TokenEnum::Base, [](CharEnum t, char) {
    return t == CharEnum::Blank;
  }));
  tsIdent.addRule(MathRule(TokenEnum::Identifier, [](CharEnum t, char s) {
    return t == CharEnum::Word || t == CharEnum::Number || s == '_';
  }));
  tsIdent.addRule(MathRule(TokenEnum::Operator, [](CharEnum t, char s) {
    return t == CharEnum::Symbol && s != '_' && s != '.';
  }));

  MathState tsOperator(TokenEnum::Operator);
  tsOperator.addRule(MathRule(TokenEnum::Base, [](CharEnum t, char) {
    return t == CharEnum::Blank;
  }));
  tsOperator.addRule(MathRule(TokenEnum::Literal, [](CharEnum t, char) {
    return t == CharEnum::Number;
  }));
  tsOperator.addRule(MathRule(TokenEnum::Identifier, [](CharEnum t, char s) {
    return t == CharEnum::Word || s == '_';
  }));
  tsOperator.addRule(MathRule(TokenEnum::Operator, [](CharEnum t, char s) {
    return t == CharEnum::Symbol && s != '_' && s != '.';
  }));

  stateSet.insert({TokenEnum::Base, tsBase});
  stateSet.insert({TokenEnum::Literal, tsLiteral});
  stateSet.insert({TokenEnum::Identifier, tsIdent});
  stateSet.insert({TokenEnum::Operator, tsOperator});

  return TokenEnum::Base;
}

} // namespace math
} // namespace prs
