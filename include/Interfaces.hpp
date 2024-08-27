#pragma once

template <class U, class V>
class Divisible {
public:
  virtual U operator/(V rhs) = 0;
};

template <class U, class V>
class Arithmetic {
public:
  virtual U operator+(V rhs) = 0;
  virtual U operator-(V rhs) = 0;
  virtual U operator-() = 0;
  virtual U operator*(V rhs) = 0;
};

template <class U, class V>
class Comparable {
public:
  virtual U operator==(V rhs) = 0;
  virtual U operator<=(V rhs) = 0;
  virtual U operator>=(V rhs) = 0;
  virtual U operator<(V rhs) = 0;
  virtual U operator>(V rhs) = 0;
};
