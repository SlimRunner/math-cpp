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

} // namespace err
