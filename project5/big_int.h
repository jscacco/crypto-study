// Name: Jack Scacco
// Date: 11/11/19
// File: big_int.h
// Project: RSA Implementation
// Purpose: Define a big int class which can be used for math in rsa.cpp

#include<vector>

#ifndef BIG_INT_H
#define BIG_INT_H

class BigInt {
 public:
  BigInt(string digits);
  void display(std::ostream& stream) const;
 private:
  vector<size_t> number;
  size_t num_digits;
};

#endif
