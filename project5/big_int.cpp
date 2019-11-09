// Name: Jack Scacco
// Date: 11/11/19
// File: big_int.cpp
// Project: RSA Implementation
// Purpose: Implement the BigInt class

#include <iostream>
#include <vector>

#include "big_int.h"

using namespace std;

/* Fill number with the digits of the input string */
BigInt::BigInt(string digits) {
  for (size_t i = digits.size() - 1; i >=0; i--) numbers.pushback(digits[i]);
}

BigInt::void display(std::ostream& stream) const {}

vector<size_t> number;
size_t num_digits;
};
