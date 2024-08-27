#pragma once

#include <cstddef> // size_t
#include <exception>

namespace err {

class parse_error : public std::exception {
public:
  parse_error();
  parse_error(std::size_t at);

  virtual char const *what() const noexcept;
  virtual std::size_t at() const noexcept;

protected:
  std::size_t m_pos;
};

class invalid_symbol : public parse_error {
  public:
  invalid_symbol();
  invalid_symbol(std::size_t at);
  invalid_symbol(std::size_t at, char symbol);

  virtual char const *what() const noexcept;
  virtual char which() const noexcept;
protected:
  char m_sym;
};

class unexpected_symbol : public parse_error {
  public:
  unexpected_symbol();
  unexpected_symbol(std::size_t at);
  unexpected_symbol(std::size_t at, char symbol);

  virtual char const *what() const noexcept;
  virtual char which() const noexcept;
protected:
  char m_sym;
};

} // namespace err
