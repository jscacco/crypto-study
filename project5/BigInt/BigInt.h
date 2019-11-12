// Name: Jack Scacco
// Date: 11/11/19
// File: big_int.h
// Project: RSA Implementation
// Purpose: Define a big int class which can be used for math in rsa.cpp

#include <deque>
#include <string>

#ifndef BIGINT_H
#define BIGINT_H

size_t MAX_1024_BIT_VAL_LEN = 308;

class BigInt {
 public:
  size_t num_digits;
  std::vector<size_t> digits;
  bool positive;

  BigInt(std::string digits); // The normal constructor
  BigInt(const BigInt * other); // The copy constructor
  void display(std::ostream& stream);
  
  BigInt * add(BigInt * other);
  BigInt * slow_mult(BigInt * other);
  BigInt * multiply(BigInt * other);
 private:
  void trim_leading_zeros();
  void zero_out();
};

#endif
