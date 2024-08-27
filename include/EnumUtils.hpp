#include <iostream>    // ostream
#include <type_traits> // underlying_type_t

namespace enumutl {

template <class Elem, class U = std::underlying_type_t<Elem>>
constexpr U operator|(const Elem lhs, const Elem rhs) {
  static_assert(std::is_enum<Elem>::value,
                "Template parameter must be an enum type.");
  return static_cast<U>(static_cast<U>(lhs) | static_cast<U>(rhs));
}

template <class Elem, class U = std::underlying_type_t<Elem>>
constexpr U operator&(const Elem lhs, const Elem rhs) {
  static_assert(std::is_enum<Elem>::value,
                "Template parameter must be an enum type.");
  return static_cast<U>(static_cast<U>(lhs) & static_cast<U>(rhs));
}

template <class Elem, class U = std::underlying_type_t<Elem>>
constexpr U operator^(const Elem lhs, const Elem rhs) {
  static_assert(std::is_enum<Elem>::value,
                "Template parameter must be an enum type.");
  return static_cast<U>(static_cast<U>(lhs) ^ static_cast<U>(rhs));
}

template <class Elem, class U = std::underlying_type_t<Elem>>
constexpr std::ostream &operator<<(std::ostream &ostrm, const Elem other) {
  static_assert(std::is_enum<Elem>::value,
                "Template parameter must be an enum type.");
  ostrm << static_cast<U>(other);
  return ostrm;
}

} // namespace enumutl
