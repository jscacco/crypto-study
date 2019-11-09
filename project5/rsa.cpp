// Name: Jack Scacco
// Due Date: 11.15.19
// File: rsa.cpp
// Purpose: Implement RSA Encryption

#include <iostream>
#include <math.h> /* log2, sqrt */
#include <cassert> /*assert */
#include <vector>
#include <time.h> /* random seed generation */

#include "big_int.h" /* big int library */

using namespace std;

// TODO:
// 1. Determine where we need to use BigInts and where we can use size_t
// 2. Fix the seg faulting in prime_list() for large input values
// 3. Ensure that the random number generation is actually secure

/* Exponentiates using recursion. */
size_t exp_val_mod(const size_t base, const size_t exp, const size_t mod) {
  if (exp == 0) return 1;
  else if (exp == 1) return base % mod;
  else {
    if (exp % 2 == 0) {size_t recur = exp_val_mod(base, exp/2, mod); return (recur * recur) % mod;}
    else { size_t recur = exp_val_mod(base, (exp - 1)/2, mod); return (base * recur * recur) % mod; }
  }
}


/* Given an upper bound n, returns a list of all primes less than n using the Sieve of Eratosthenes */
vector<size_t> prime_list(size_t n) {

  vector<bool> is_prime (n, true);    // This is the list of bools which determines if that index + 1 is a prime
  size_t current_prime = 2;  

  while (current_prime < sqrt(n)) {
    
    // Set the appropriate values in is_prime to false
    // TODO: Determine why this line is segfaulting for larger values! (like 1234567 but not 123456)
    for (size_t i = (current_prime - 1) + current_prime; i < n; i += current_prime) is_prime[i] = false;
        
    // Set the next current_prime
    current_prime ++;
    while (!is_prime[current_prime - 1]) current_prime ++;
  }

  // Build and return the actual vector of primes
  vector<size_t> primes;
  for (size_t i = 1; i < n; i++) {
    if (is_prime[i]) primes.push_back(i + 1);
  }
  return primes;
}


/* Given N, returns True if N is a perfect power */
bool is_perfect_power(size_t n) {
  size_t bound = log2(n);
  vector<size_t> primes = prime_list(n);
  
  for (size_t i = 0; i < primes.size(); i++)
    for (size_t exp = 2; exp <= bound; exp++)
      if (exp_val_mod(primes[i], exp, UINT_MAX) == n) return true;
  return false;
}


/* Given N, compute r >= 1 and odd n such that 2^(r)u = N - 1 */
vector<size_t> gen_witness_params(size_t n) {
  // This won't work if the input is a perfect power or odd, so make sure they aren't
  assert(n % 2 == 1);
  assert(!is_perfect_power(n));
  
  // 1 < r < log2(n), and 1 < u < n / 2 (u is odd)
  for (size_t r = 1; r <= log2(n); r ++) {
    for (size_t u = 1; u <= (n / 2); u += 2) {
      if ((exp_val_mod(2, r, UINT_MAX) * u) == (n - 1)) {
	return vector<size_t> {r, u};
      }
    }
  }
  assert(false);
}

/* Returns true if val is a strong witness. */
// (A value is a strong witness if both of the following hold:
// 1. val^u != 1, -1 mod n
// 2. val^((2^i)u) != -1 mod n for all 1 < i < r-1
bool is_strong_witness(size_t val, size_t r, size_t u, size_t n) {
  // First expr from above
  size_t expr1 = exp_val_mod(val, u, n);
  if (expr1 == 1 or expr1 == n - 1) return false;

  // Second expr from above
  for (size_t i = 1; i < r; i ++) if (exp_val_mod(val, u * exp_val_mod(2, i, UINT_MAX), n) == n - 1) return false;

  return true;
}

/* Generates a psuedorandom number between lower and upper, not including upper */
int gen_rand(int lower, int upper) {
  assert (upper > lower);

  // TODO: Ensure that the random number generation is actually secure
  // ^ look at "https://en.cppreference.com/w/cpp/numeric/random"
  // Initialize a random seed to a value representing the current time.
  srand(time(NULL));
  return rand() % (upper - lower) + lower;
}


/* Computes the gcd of two values by using the Euclidean Algorithm */
size_t gcd(size_t val1, size_t val2) {
  if(val1 == val2) return val1;
  else if (val1 > val2) return gcd(val1 - val2, val2);
  else return gcd(val1, val2 - val1);
}


/* Determines if a number is prime or composite by using the Miller-Rabin test. Prime = True. */
bool miller_rabin(size_t val, size_t limit) {
  
  if ((val % 2 == 0) || is_perfect_power(val)) return false;
  
  vector<size_t> witness_params = gen_witness_params(val);
  size_t r = witness_params[0];
  size_t u = witness_params[1];

  for (size_t _ = 0; _ < limit; _ ++) {
    int attempt = gen_rand(1, val);
    if ((gcd(attempt, val) != 1) || is_strong_witness(attempt, r, u, val)) return false;
  }
  return true;
}

int main() {
  
  for (size_t val = 2; val < 100; val++) {
    cout << val << ": ";
    bool is_prime = miller_rabin(val, log(val) + 1);
    is_prime ? cout << "prime" : cout << "composite";
    cout << endl;
  }
 
  
  // prime_list(1048576);
  // prime_list(9999999);
  
  
}
  
