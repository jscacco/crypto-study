// Name: Jack Scacco
// Date: 11/11/19
// File: big_int.cpp
// Project: RSA Implementation
// Purpose: Implement the BigInt class

#include <iostream>
#include <vector>
#include <cassert>

#include "BigInt.h"

using namespace std;

BigInt::BigInt(string input) {
  assert(input.size() > 0);
  /* Fill number with the digits of the input string */
  num_digits = 0;
  for (int i = input.length() - 1; i >= 0; i--) { digits.push_back(input[i] - '0'); num_digits ++; }
  //trim_leading_zeros();
}

BigInt::BigInt(const BigInt * other) {
  num_digits = other->num_digits;
  for (int i = 0; i < other->num_digits; i ++) digits.push_back(other->digits[i]);
  //trim_leading_zeros();
}

/* Removes any leading zeros from the big int. */
void BigInt::trim_leading_zeros() {
  while (digits[digits.size() - 1] == 0) { digits.pop_back(); num_digits --; }
}

void BigInt::display(ostream& stream) {
  //trim_leading_zeros();
  for (int i = digits.size() - 1; i >=0; i--) stream << digits[i];
  cout << endl;
}

BigInt * BigInt::add(BigInt * other) {
  
  BigInt * this_num = nullptr;
  BigInt * that_num = nullptr;
  if (num_digits < other->num_digits) {
    this_num = new BigInt(this);
    that_num = other;
  }
  else {
    this_num = new BigInt(other);
    that_num = this;
  }
  
  // Make sure the numbers have the same amount of digits in them
  while (this_num->num_digits < that_num->num_digits) {
    this_num->digits.push_back(0);
    this_num->num_digits ++;
  }

  // Actually add the numbers together
  size_t carry = 0;
  for (size_t i = 0; i < this_num->num_digits; i++){
    this_num->digits[i] += that_num->digits[i] + carry;
    carry =  this_num->digits[i] > 9;
    this_num->digits[i] %= 10;
  }
  if (carry == 1) {
    this_num->digits.push_back(carry);
    this_num->num_digits ++;
  }
  return this_num;
}

BigInt * BigInt::multiply(BigInt * other) {

  BigInt * smaller = (num_digits < other->num_digits) ? this : other;
  BigInt * larger = (smaller == this) ? other : this;
  
  BigInt * this_num = new BigInt("0");
  size_t multiplier = 1;

  for (size_t i = 0; i < smaller->num_digits; i++){
    for (size_t j = 0; j < (multiplier * smaller->digits[i]); j++) {
	BigInt * temp = this_num;
	this_num = this_num->add(larger);
	delete temp;
      }
    multiplier *= 10;
  }
  return this_num;
}


