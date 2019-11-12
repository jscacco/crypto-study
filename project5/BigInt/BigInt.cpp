// Name: Jack Scacco
// Date: 11/11/19
// File: big_int.cpp
// Project: RSA Implementation
// Purpose: Implement the BigInt class

#include <iostream>
#include <deque>
#include <cassert>
#include <math.h>

#include "BigInt.h"

using namespace std;

size_t MAX_1024_BIT_VAL_LEN = 308;

BigInt::BigInt(string input) {
  assert(input.size() > 0 && input != "-");
  /* Fill number with the digits of the input string */
  num_digits = 0;
  for (int i = input.length() - 1; i > 0; i--) { digits.push_back(input[i] - '0'); num_digits ++; }
  if (input[0] == '-') positive = false;
  else {
    positive = true;
    digits.push_back(input[0] - '0');
    num_digits ++; }
  // trim_leading_zeros();
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


void BigInt::zero_out(){
  for (size_t i = 0; i < num_digits; i++) digits[i] = 0;
}


void BigInt::display(ostream& stream) {
  // trim_leading_zeros();
  for (int i = digits.size() - 1; i >=0; i--) stream << digits[i];
  cout << endl;
}


/* Adds other to this. */
BigInt * BigInt::add(BigInt * other) {

  // Since we zero_out() 'this', the algorithm won't work if 'this' is passed as the argument.
  if (this == other) other = new BigInt(this);
  
  // Make sure the numbers have the same amount of digits in them
  while (num_digits < other->num_digits) {
    digits.push_back(0);
    num_digits ++;
  }
  while (num_digits > other->num_digits) {
    other->digits.push_back(0);
    other->num_digits ++;
  }

  // Actually add the numbers together
  size_t carry = 0;
  for (size_t i = 0; i < num_digits; i++){
    digits[i] += other->digits[i] + carry;
    carry = digits[i] > 9;
    digits[i] %= 10;
  }
  if (carry == 1) {
    digits.push_back(carry);
    num_digits ++;
  }
  return this;
}


BigInt * BigInt::slow_mult(BigInt * other) {

  BigInt * smaller = nullptr;
  BigInt * larger = nullptr;
  
  if (num_digits < other->num_digits) {
    smaller = this;
    larger = other;
  }
  else {
    smaller = other;
    larger = this;
  }
  
  BigInt * this_num = new BigInt("0");
  size_t multiplier = 1;

  for (size_t i = 0; i < smaller->num_digits; i++){
    for (size_t j = 0; j < (multiplier * smaller->digits[i]); j++) this_num->add(larger);
    multiplier *= 10;
  }
  return this_num;
}


BigInt * BigInt::multiply(BigInt * other) {
  
  // Base case
  if (num_digits < 10 || other->num_digits < 10) return slow_mult(other);
  
  else {
    if (num_digits % 2 == 1) { digits.push_back(0); num_digits++; }
    if (other->num_digits % 2 == 1) { other->digits.push_back(0); other->num_digits++; }
    // Split each value in half
    string this_front, this_back, other_front, other_back;
    for (int i = num_digits - 1; i > num_digits / 2 - 1;  i-- )
      this_front += char(digits[i] + '0'); 
    for (int i = num_digits / 2 - 1; i >= 0; i-- )
      this_back += char(digits[i] + '0');
    for (int i = other->num_digits - 1; i > other->num_digits / 2 - 1;  i-- )
      other_front += char(other->digits[i] + '0'); 
    for (int i = other->num_digits / 2 - 1; i >= 0; i-- )
      other_back += char(other->digits[i] + '0');

    BigInt * this1 = new BigInt(this_front);
    BigInt * this2 = new BigInt(this_back);
    BigInt * other1 = new BigInt(other_front);
    BigInt * other2 = new BigInt(other_back);

    BigInt * term_one = this1->multiply(other1);
    for (size_t i = 0; i <  this1->num_digits + other1->num_digits; i ++) {
      term_one->digits.push_front(0);
      term_one->num_digits ++;
    }
    BigInt * term_two = this1->multiply(other2);
    for (size_t i = 0; i <  this1->num_digits; i ++) {
      term_two->digits.push_front(0);
      term_two->num_digits ++;
    }
    BigInt * term_three = this2->multiply(other1);
    for (size_t i = 0; i <  other1->num_digits; i ++) {
      term_three->digits.push_front(0);
      term_three->num_digits ++;
    }
    BigInt * term_four = this2->multiply(other2);
    
    return term_one->add(term_two->add(term_three->add(term_four)));
  }
}

