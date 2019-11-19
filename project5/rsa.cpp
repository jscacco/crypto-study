// Name: Jack Scacco
// Due Date: 11.15.19
// File: rsa.cpp
// Purpose: Implement RSA Encryption

#include <iostream>
#include <math.h> /* log2, sqrt */
#include <cassert> /*assert */
#include <vector>
#include <time.h> /* random seed generation */
#include <cstdlib>
#include <climits>
#include <random> /* random bit generation */

#include <stdio.h> /* These two are for GMP */
#include <gmp.h>

using namespace std;

// TODO:
// 1. Determine where we need to use BigInts and where we can use size_t
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


// /* Given an upper bound n, returns a list of all primes less than n using the Sieve of Eratosthenes */
// vector<size_t> prime_list(size_t n) {
// 
// vector<bool> is_prime (n, true);    // This is the list of bools which determines if that index + 1 is a prime
// size_t current_prime = 2;  
// 
// while (current_prime < sqrt(n)) {
// 
// // Set the appropriate values in is_prime to false
// // TODO: Determine why this line is segfaulting for larger values! (like 1234567 but not 123456)
// for (size_t i = (current_prime - 1) + current_prime; i < n; i += current_prime) is_prime[i] = false;
// 
// // Set the next current_prime
// current_prime ++;
// while (!is_prime[current_prime - 1]) current_prime ++;
// }

// // Build and return the actual vector of primes
// vector<size_t> primes;
// for (size_t i = 1; i < n; i++) {
// if (is_prime[i]) primes.push_back(i + 1);
// }
// return primes;
// }


// /* Given N, returns True if N is a perfect power */
// bool is_perfect_power(size_t n) {
// size_t bound = log2(n);
// vector<size_t> primes = prime_list(n);
// 
// for (size_t i = 0; i < primes.size(); i++)
// for (size_t exp = 2; exp <= bound; exp++)
// if (exp_val_mod(primes[i], exp, UINT_MAX) == n) return true;
// return false;
// }


// Everything above this line can be done with the GMP library.
// Well, so can miller-rabin, but that would be egregious


// /* Given N, compute r >= 1 and odd n such that 2^(r)u = N - 1 */
// vector<size_t> gen_witness_params(size_t n) {
//   // This won't work if the input is a perfect power or odd, so make sure they aren't
//   assert(n % 2 == 1);
//   assert(!is_perfect_power(n));
//   
//   // 1 < r < log2(n), and 1 < u < n / 2 (u is odd)
//   for (size_t r = 1; r <= log2(n); r ++) {
//     for (size_t u = 1; u <= (n / 2); u += 2) {
//       if ((exp_val_mod(2, r, UINT_MAX) * u) == (n - 1)) {
// 	return vector<size_t> {r, u};
//       }
//     }
//   }
//   assert(false);
// }


/* Sets half to input/2 */
void mpz_half(mpz_t half, mpz_t input) {
  mpz_t two;
  mpz_init(two);
  mpz_set_ui(two, 2);
  mpz_tdiv_q(half, input, two);
  mpz_clear(two);
}


/* Given N, compute r >= 1 and odd n such that 2^(r)u = N - 1 */
void gen_witness_params(mpz_t r_out, mpz_t u_out, mpz_t n) {
  // This won't work if the input is a perfect power or odd, so make sure they aren't
  
  assert(mpz_odd_p(n) != 0);   // Ensure n is odd
  assert(mpz_perfect_power_p(n) == 0); // Ensure n isn't a perfect power

  // We also need to know n / 2
  mpz_t half_n, temp1, temp2;
  mpz_init(half_n);
  mpz_init(temp1);
  mpz_init(temp2);
  mpz_half(half_n, n);

  // GMP has no logarithm functions, but we can figure that out ourselves.
  size_t log2_ceil = mpz_sizeinbase(n, 2);

  // 1 < r < log2(n), and 1 < u < n / 2 (u is odd)
  for (size_t r = 1; r <= log2_ceil; r ++) {
    mpz_set_ui(u_out, 1);

    // TODO: This loop takes forever with large numbers. How to speed it up?
    while(mpz_cmp(u_out, half_n) < 1) {
      mpz_mul_ui(temp1, u_out, exp_val_mod(2, r, UINT_MAX)); // u(2^r)
      mpz_sub_ui(temp2, n, 1); // (n - 1)
      if (mpz_cmp(temp1, temp2) == 0) {
	mpz_clear(half_n);
	mpz_clear(temp1);
	mpz_clear(temp2);
	mpz_set_ui(r_out, r);
	return;
      }
      mpz_add_ui(u_out, u_out, 2);
    }
  }
  assert(false);
}


// /* Returns true if val is a strong witness. */
// (A value is a strong witness if both of the following hold:
// 1. val^u != 1, -1 mod n
// 2. val^((2^i)u) != -1 mod n for all 1 < i < r-1
// bool is_strong_witness(size_t val, size_t r, size_t u, size_t n) {
//   // First expr from above
//   size_t expr1 = exp_val_mod(val, u, n);
//   if (expr1 == 1 or expr1 == n - 1) return false;
//
//   // Second expr from above
//   for (size_t i = 1; i < r; i ++) if (exp_val_mod(val, u * exp_val_mod(2, i, UINT_MAX), n) == n - 1) return false;
// 
//   return true;
// }


/* Sets result to 2^exp % mod */
void mpz_exp_2(mpz_t result, mpz_t exp, mpz_t mod) {
  mpz_t two;
  mpz_init(two);
  mpz_set_ui(two, 2);
  mpz_powm(result, two, exp, mod);
  mpz_clear(two);
}


/* Returns true if val is a strong witness. */
// (A value is a strong witness if both of the following hold:
// 1. val^u != 1, -1 mod n
// 2. val^((2^i)u) != -1 mod n for all 1 < i < r-1
bool is_strong_witness(mpz_t val, mpz_t r, mpz_t u, mpz_t n) {
  // First expr from above
  
  mpz_t expr1, n_min_1, temp1;
  mpz_init(expr1);
  mpz_init(n_min_1);
  mpz_init(temp1);
  
  mpz_sub_ui(n_min_1, n, 1); // n_min = (n - 1)
  mpz_powm(expr1, val, u, n); // expr1 = val^u mod n
  
  if (mpz_cmp_ui(expr1, 1) == 0 || mpz_cmp(expr1, n_min_1) == 0) return false;
  // Second expr from above
  mpz_t i;
  mpz_init(i);
  mpz_set_ui(i, 1);
  while (mpz_cmp(i, r) < 1) { // for (i = 1; i < r; i ++)
    mpz_exp_2(temp1, i, n); // temp1 = 2 ^ i mod n
    mpz_mul(temp1, u, temp1); // temp1 = u(2^i) mod n
    mpz_powm(temp1, val, temp1, n); // temp1 = val^(u(2^i)) mod n
    if (mpz_cmp(temp1, n_min_1) == 0) { // if val^(u(2^i)) mod n = -1
      mpz_clear(expr1);
      mpz_clear(n_min_1);
      mpz_clear(temp1);
      mpz_clear(i);
      return false;
    }
    mpz_add_ui(i, i, 1);
  }
  mpz_clear(expr1);
  mpz_clear(n_min_1);
  mpz_clear(temp1);
  mpz_clear(i);
  return true;
}


// /* Generates a psuedorandom number between lower and upper, not including upper */
// int gen_rand(int lower, int upper) {
//   assert (upper > lower);
// 
//   // TODO: Ensure that the random number generation is actually secure
//   // ^ look at "https://en.cppreference.com/w/cpp/numeric/random"
//   // Initialize a random seed to a value representing the current time.
//   srand(time(NULL));
//   return rand() % (upper - lower) + lower;
// }


// /* Computes the gcd of two values by using the Euclidean Algorithm */
// size_t gcd(size_t val1, size_t val2) {
//   if(val1 == val2) return val1;
//   else if (val1 > val2) return gcd(val1 - val2, val2);
//   else return gcd(val1, val2 - val1);
// }


// /* Determines if a number is prime or composite by using the Miller-Rabin test. Prime = True. */
// bool miller_rabin(size_t val, size_t limit) {
//   if ((val % 2 == 0) || is_perfect_power(val)) return false;
//   
//   vector<size_t> witness_params = gen_witness_params(val);
//   size_t r = witness_params[0];
//   size_t u = witness_params[1];
// 
//   for (size_t _ = 0; _ < limit; _ ++) {
//     int attempt = gen_rand(1, val);
//     if ((gcd(attempt, val) != 1) || is_strong_witness(attempt, r, u, val)) return false;
//   }
//   return true;
// }


/* Determines if a number is prime or composite by using the Miller-Rabin test. Prime = True. */
bool miller_rabin(mpz_t val, const size_t limit) {
  // CITE: https://stackoverflow.com/questions/30942413/c-gmp-generating-random-number
  // DESC: How to properly do random number generation

  if (mpz_cmp_ui(val, 2) == 0) return true;
  if ((mpz_even_p(val) != 0) || mpz_perfect_power_p(val) != 0) return false;

  mpz_t r, u;
  mpz_init(r);
  mpz_init(u);
  gen_witness_params(r, u, val);
  
  mpz_t attempt, this_gcd, upper_bound;
  mpz_init(attempt);
  mpz_init(this_gcd);
  mpz_init(upper_bound);
  unsigned long seed;

  mpz_sub_ui(upper_bound, val, 1);

  gmp_randstate_t rstate;
  gmp_randinit_mt(rstate);
  gmp_randseed_ui(rstate, seed);

  for (size_t _ = 0; _ < limit; _++) {
    mpz_urandomm(attempt, rstate, upper_bound);
    mpz_add_ui(attempt, attempt, 1);

    mpz_gcd(this_gcd, val, attempt);
    if ((mpz_cmp_ui(this_gcd, 1) != 0) || is_strong_witness(attempt, r, u, val)) {
      mpz_clear(r);
      mpz_clear(u);
      mpz_clear(attempt);
      mpz_clear(this_gcd);
      mpz_clear(upper_bound);
      gmp_randclear(rstate);
      return false;
    }
  }
  mpz_clear(r);
  mpz_clear(u);
  mpz_clear(attempt);
  mpz_clear(this_gcd);
  mpz_clear(upper_bound);
  gmp_randclear(rstate);
  return true;
}

// Return true if attempt^(val-1/2)mpd val is 1 or val-1
bool strong_prime_test(mpz_t attempt, mpz_t val) {

  mpz_t temp1;
  mpz_t temp2;
  mpz_init(temp1);
  mpz_init(temp2);

  mpz_sub_ui(temp1, val, 1);
  mpz_div_ui(temp1, temp1, 2);
  mpz_powm(temp1, attempt, temp1, val);

  mpz_sub_ui(temp2, val, 1);
  
  if (mpz_cmp_ui(temp1, 1) == 0 || mpz_cmp(temp1, temp2) == 0) {
    mpz_clear(temp1);
    mpz_clear(temp2);
    return true;
  }
  
  mpz_clear(temp1);
  mpz_clear(temp2);
  return false;
}


/*Returns True if the passed value is prime */
bool is_probable_prime(mpz_t val, size_t limit) {
  if (mpz_cmp_ui(val, 2) == 0) return true;
  else if (mpz_even_p(val) != 0) return false;
  else {
    mpz_t attempt, this_gcd, upper_bound;
    mpz_init(attempt);
    mpz_init(this_gcd);
    mpz_init(upper_bound);
    unsigned long seed;
    
    mpz_sub_ui(upper_bound, val, 2);
    
    gmp_randstate_t rstate;
    gmp_randinit_mt(rstate);
    gmp_randseed_ui(rstate, seed);
    
    for (size_t _ = 0; _ < limit; _++) {
      mpz_urandomm(attempt, rstate, upper_bound);
      mpz_add_ui(attempt, attempt, 2);
      
      mpz_gcd(this_gcd, val, attempt);
      if ((mpz_cmp_ui(this_gcd, 1) != 0) || !strong_prime_test(attempt, val)) {
	mpz_clear(attempt);
	mpz_clear(this_gcd);
	mpz_clear(upper_bound);
	gmp_randclear(rstate);
	return false;
      }
    }
    mpz_clear(attempt);
    mpz_clear(this_gcd);
    mpz_clear(upper_bound);
    gmp_randclear(rstate);
  }
  return true;
}


/* Sets prime to a bits-bit long prime number. */
void mpz_gen_prime(mpz_t prime, size_t bits) {
  assert(bits > 1);
  // Prepare the random seed and generator
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(0, 1);

  char digits [bits + 1];
  char * digits_head = & digits[0];

  // TODO: Figure out how many times we should actually do this (it's just an undefined c in the book)
  for (size_t i = 0; i < 1000; i++) {
    
    // Randomly generate the bits
    // digits[0] = '1';
    mpz_set_ui(prime, 1);
    for (size_t b = 1; b < bits; b ++) {
      mpz_mul_ui(prime, prime, 2); // shift left
      mpz_add_ui(prime, prime, (size_t)(dis(gen)*10) % 2);
    }
    // digits[bits] = 0;

    // Check to see if the bits are prime
    if (is_probable_prime(prime, 1000)) return;
  }
  assert(false);
}


/* Sets n to be a bits-bit long value with prime factors p, q. */
void gen_modulus(mpz_t n, mpz_t p, mpz_t q, size_t bits) {
  size_t p_len = bits/2;
  size_t q_len = bits - p_len;

  mpz_gen_prime(p, p_len);
  assert(mpz_probab_prime_p(p, 50) != 0);
  mpz_gen_prime(q, q_len);
  assert(mpz_probab_prime_p(q, 50) != 0);
  mpz_mul(n, p, q);

}


void gen_rsa(mpz_t n, mpz_t e, mpz_t d, size_t bits) {
  mpz_t p, q;
  mpz_init(p);
  mpz_init(q);

  gen_modulus(n, p, q, bits);

  mpz_t phi_n;
  mpz_init(phi_n);

  mpz_sub_ui(p, p, 1); // (p - 1)
  mpz_sub_ui(q, q, 1); // (q - 1)
  mpz_mul(phi_n, p, q); // (p - 1)(q - 1)

  // TODO: is this a legit choice for e?
  // Set e
  mpz_set_ui(e, 65537);

  // Set d
  mpz_invert(d, e, phi_n);
  
  mpz_clear(phi_n);
  mpz_clear(q);
  mpz_clear(p);
}


/* Encrypt message m using public key <N, e>. Store the result in c */
void encrypt(mpz_t m, mpz_t c, mpz_t e, mpz_t n){
  mpz_powm(c, m, e, n);
}

/* Decrypt ciphertext c using private key <N, d>. Store the result in m */
void decrypt(mpz_t c, mpz_t m, mpz_t d, mpz_t n){
  mpz_powm(m, c, d, n);
}
  
int main() {
  
  // Test gen_prime
  /*
  mpz_t test;
  mpz_init(test);
  mpz_gen_prime(test, 512);

  char digits[512];
  cout << mpz_get_str(digits, 10, test) << endl;
  */


  // Test is_probable_prime
  /*
  mpz_t prime;
  mpz_init(prime);
  for (size_t i = 2; i < 100; i++){
    mpz_set_ui(prime, i);
    cout << to_string(i) << ": ";
    if(is_probable_prime(prime, 50)) cout << "prime" << endl;
    else cout << "composite" << endl;
  }
  */
  
  // Test gen_modulus
  /*
  mpz_t n, p, q;
  mpz_init(n);
  mpz_init(p);
  mpz_init(q);

  gen_modulus(n, p, q, 1024);

  char digits[1024];
  
  cout << "N: " << mpz_get_str(digits, 2, n) << endl;
  cout << "p: " << mpz_get_str(digits, 2, p) << endl;
  cout << "q: " << mpz_get_str(digits, 2, q) << endl;
  */

  // Test GenRSA
  mpz_t n, e, d;
  mpz_init(n);
  mpz_init(e);
  mpz_init(d);

  gen_rsa(n, e, d, 2048);

  mpz_t m1, m2, c;
  mpz_init(m1);
  mpz_init(m2);
  mpz_init(c);

  char digits[1024];

  mpz_set_ui(m1, 5221999);

  cout << "Original message: " << mpz_get_str(digits, 10, m1) << endl;
  encrypt(m1, c, e, n);
  cout << "Ciphertext: " << mpz_get_str(digits, 10, c) << endl;
  decrypt(c, m2, d, n);
  cout << "Decoded message: " << mpz_get_str(digits, 10, m2) << endl;
}
  
