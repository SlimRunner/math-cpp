#include "Exceptions.hpp"

#include <limits>
#include <string_view>

namespace err {

parse_error::parse_error() : m_pos{std::numeric_limits<std::size_t>::max()} {}

parse_error::parse_error(std::size_t at) : m_pos{at} {}

char const *parse_error::what() const noexcept {
  return "Parse error occurred.";
}

std::size_t parse_error::at() const noexcept { return m_pos; }

} // namespace err
