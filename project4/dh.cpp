// Name: Jack Scacco
// Due Date: 10.25.19
// File: df.cpp
// Purpose: Implement Diffie-Hellman key exchange

#include <iostream>
#include <math.h> /* pow */
#include <cassert> /*assert */

using namespace std;

/* Exponentiates using recursion. */
/* Update 11.4.19: This works, but can be made more efficient. */
// size_t exp_val(const size_t base, const size_t exp, const size_t mod) {
//   if (exp < 1) {
//     return base % mod;
//   }
//   return (base * exp_val(base, exp - 1, mod)) % mod;
// }


/* Exponentiates using recursion. */
size_t exp_val_mod(const size_t base, const size_t exp, const size_t mod) {
  if (exp == 0) return 1;
  else if (exp == 1) return base % mod;
  else {
    if (exp % 2 == 0) {size_t recur = exp_val_mod(base, exp/2, mod); return (recur * recur) % mod;}
    else { size_t recur = exp_val_mod(base, (exp - 1)/2, mod); return (base * recur * recur) % mod; }
  }
}


size_t gen_val(const size_t prime, const size_t root, const size_t key){
  return exp_val(root, key, prime);  
}


size_t dec_val(const size_t prime, const size_t my_key, const size_t their_val){
  return exp_val(their_val, my_key, prime);
}


int main() {
  /* These values are public. */
  const size_t prime = 23;
  const size_t primitive_root = 5;
  
  /* These values are private. */
  const size_t alice_key = 4;
  const size_t bob_key = 3;
  
  /* Each party computes a value to be transmitted. */
  const size_t alice_val = gen_val(prime, primitive_root, alice_key);
  const size_t bob_val = gen_val(prime, primitive_root, bob_key);
  
  /* Each party determines the shared key from the transmitted values. */
  const size_t alice_shared = dec_val(prime, alice_key, bob_val);
  const size_t bob_shared = dec_val(prime, bob_key, alice_val);
  
  /* Verfiy */
  assert(alice_shared == bob_shared);
  cout << "Success! Both parties have computed the same shared key." << endl;

}
  
