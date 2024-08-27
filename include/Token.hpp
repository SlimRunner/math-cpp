#pragma once

#include <functional>
#include <string>
#include <variant>
#include <vector>

namespace prs {

enum class TokenType {
  Literal,
  Operator,
  Identifier,
};

class Token {
private:
  std::string m_payload;
  TokenType m_type;

public:
  Token() = delete;
  Token(std::string data, TokenType type);

  std::string payload() const;
  TokenType type() const;
};

std::vector<Token> tokenize(const std::string & exprstr);

} // namespace prs

// enum class OperatorType {
//   PLUS,
//   MINUS,
//   STAR,
//   SLASH,
//   EQUAL,
//   GT,
//   GEQ,
//   LT,
//   LEQ,
//   LPAREN,
//   RPAREN,
// };

// template <class T> struct Literal {
//   T value;
// };

// struct Operator {
//   OperatorType value;
// };

// template <class T> struct Constant {
//   std::string name;
//   T value;
// };

// template <class T> struct Variable {
//   std::string name;
// };

// template <class T> class Token;
// template <class T> struct Function {
//   std::string name;
//   std::vector<Token<T>> params;
// };

// template <class T>
// using TokenValue =
//     std::variant<Literal<T>, Operator, Constant<T>, Variable<T>, Function<T>>;
