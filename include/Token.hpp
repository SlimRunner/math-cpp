#pragma once

#include <algorithm>  // copy
#include <cstddef>    // size_t
#include <functional> // function
#include <initializer_list>
#include <iterator> // inserter
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace prs {

template <class CSet> class SimplType {
private:
  const CSet m_type;
  std::set<char> m_charset;

public:
  SimplType() = delete;
  SimplType(CSet type, std::initializer_list<char> initlist)
      : m_type{type}, m_charset{initlist} {}
  SimplType(CSet type, std::string charset) : m_type{type}, m_charset{} {
    std::copy(charset.cbegin(), charset.cend(),
              std::inserter(m_charset, m_charset.cend()));
  }
  template <class It>
  SimplType(CSet type, It start, It end) : m_type{type}, m_charset{} {
    std::copy(start, end, std::inserter(m_charset, m_charset.cend()));
  }

  CSet type() const { return m_type; }

  std::set<char>::iterator begin() const { return m_charset.cbegin(); }

  std::set<char>::iterator end() const { return m_charset.cend(); }

  struct TypeSort {
    bool operator()(const SimplType<CSet> &lhs,
                    const SimplType<CSet> &rhs) const {
      return lhs.m_type < rhs.m_type;
    }
  };
};

template <class TSet, class CSet> class TokenRule {
private:
  using Eval = std::function<bool(CSet type, char sym)>;

  Eval m_evaluator;
  const TSet m_target;
  std::size_t m_count;
  const std::size_t m_limit;

public:
  TokenRule() = delete;
  TokenRule(TSet target, Eval rule)
      : m_evaluator{rule}, m_target{target}, m_count{0}, m_limit{0} {}
  TokenRule(TSet target, Eval rule, std::size_t limit)
      : m_evaluator{rule}, m_target{target}, m_count{0}, m_limit{limit} {}

  CSet target() const { return m_target; }

  std::optional<TSet> operator()(CSet type, char sym) {
    if ((m_limit == 0 || m_count < m_limit) && this->m_evaluator(type, sym)) {
      ++m_count;
      return m_target;
    }
    return {};
  }
};

template <class TSet, class CSet> class TokenState {
private:
  using RuleSet = std::vector<TokenRule<TSet, CSet>>;

  const TSet m_type;
  RuleSet m_ruleset;

public:
  TokenState() = delete;
  TokenState(TSet type) : m_type{type} {}

  CSet type() const { return m_type; }

  bool isTerminal() const { return !m_ruleset.size(); }

  void addRule(TokenRule<TSet, CSet> rule) { m_ruleset.push_back(rule); }

  typename RuleSet::iterator begin() { return m_ruleset.begin(); }

  typename RuleSet::iterator end() { return m_ruleset.end(); }

  typename RuleSet::iterator begin() const { return m_ruleset.cbegin(); }

  typename RuleSet::iterator end() const { return m_ruleset.cend(); }

  struct TypeSort {
    bool operator()(const TokenState<TSet, CSet> &lhs,
                    const TokenState<TSet, CSet> &rhs) const {
      return lhs.m_type < rhs.m_type;
    }
  };
};

enum class TokenType {
  Literal,
  Operator,
  Identifier,
};

template <class TSet> class Token {
private:
  std::string m_payload;
  TSet m_type;
  std::size_t m_pos;

public:
  Token() = delete;
  Token(std::string data, TSet type)
      : m_payload{data}, m_type{type}, m_pos{0} {}
  Token(std::string data, TSet type, std::size_t pos)
      : m_payload{data}, m_type{type}, m_pos{pos} {}

  std::string payload() const { return m_payload; }
  TSet type() const { return m_type; }
  std::size_t position() const { return m_pos; }
};

} // namespace prs

/*
Proposed new approach: use an FSM
- all states will have a set of valid transitions and there should be no
  overlap between transitions
- a transition might have extra rules like how many times you can use it
  and also where in it's current iteration is valid (i.e. start, middle,
  or end).
- there should always be a neutral starting state, an error state, and a
  terminating state
- the final result can be refined further at the end. Such as
  dissambiguating punctuation from operators out from symbols (if
  needed).
*/
