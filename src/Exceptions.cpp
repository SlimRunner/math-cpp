#include "Exceptions.hpp"

#include <limits>
#include <string_view>

namespace err {

parse_error::parse_error() : m_pos{std::numeric_limits<std::size_t>::max()} {}

parse_error::parse_error(std::size_t at) : m_pos{at} {}

char const *parse_error::what() const noexcept {
  return "Parse error occurred";
}

std::size_t parse_error::at() const noexcept { return m_pos; }

invalid_symbol::invalid_symbol() : invalid_symbol::parse_error(), m_sym{'\0'} {}

invalid_symbol::invalid_symbol(std::size_t at)
    : invalid_symbol::parse_error(at), m_sym{'\0'} {}

invalid_symbol::invalid_symbol(std::size_t at, char symbol)
    : invalid_symbol::parse_error(at), m_sym{symbol} {}

char const *invalid_symbol::what() const noexcept { return "Invalid symbol"; }

char invalid_symbol::which() const noexcept { return m_sym; }

unexpected_symbol::unexpected_symbol()
    : unexpected_symbol::parse_error(), m_sym{'\0'} {}

unexpected_symbol::unexpected_symbol(std::size_t at)
    : unexpected_symbol::parse_error(at), m_sym{'\0'} {}

unexpected_symbol::unexpected_symbol(std::size_t at, char symbol)
    : unexpected_symbol::parse_error(at), m_sym{symbol} {}

char const *unexpected_symbol::what() const noexcept {
  return "Unexpected symbol encountered";
}

char unexpected_symbol::which() const noexcept { return m_sym; }

} // namespace err
