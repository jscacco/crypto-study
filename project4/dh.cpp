// Name: Jack Scacco
// Due Date: 10.25.19
// File: df.cpp
// Purpose: Implement Diffie-Hellman key exchange

#include <iostream>
#include <math.h> /* pow */
#include <cassert> /*assert */

using namespace std;

size_t exp_val(const size_t base, const size_t exp, const size_t prime){
  return (size_t)pow((double)base, (double)exp) % prime;
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
  
