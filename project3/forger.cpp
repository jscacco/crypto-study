#include "sha1.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using std::string;
using std::cout;
using std::endl;

static const size_t BLOCK_SIZE_INTS = 16;  /* number of 32bit integers per SHA1 block */
static const size_t BLOCK_SIZE_BYTES = BLOCK_SIZE_INTS * 4;
static const size_t BITS_PER_BYTE = 8;


/* This is the code provided in the original main function of the demo. */
string MAC(const string input) {
  SHA1 checksum;
  checksum.update(input);
  const string mac = checksum.final();
  return mac;
}


/* Takes a key and a message and hashes the concatenation of the two. */
string secret_prefix_MAC(const string key, const string input) {
  const string concat = key + input;
  return MAC(concat);
}


/* Converts a str containing hexadecimal values to the corresponding hex value. */
int hexstr_to_val(string input) {
  // Code pulled from stack overflow
  std::stringstream str;
  str << input;
  uint value;
  str >> std::hex >> value;
  return value;
}


/* Takes a message length and pads it in the same fasion as the SHA1 hash. */
string pad(const size_t msg_len) {
  const size_t len = msg_len % BLOCK_SIZE_BYTES;
  const size_t length_buffer = 8;
  string buffer = "";
  
  buffer += (char)0x80;
  while (buffer.size() < (BLOCK_SIZE_BYTES - len - length_buffer)) {
    buffer += (char)0x00;
  }

  // Append msg_len in bits
  const uint64_t msg_bits = (uint64_t)(msg_len * BITS_PER_BYTE);

  // Start at 56 because we want the first 8 bits of the 64-but length
  buffer += (char)((uint8_t)(msg_bits >> 56));
  buffer += (char)((uint8_t)(msg_bits >> 48));
  buffer += (char)((uint8_t)(msg_bits >> 40));
  buffer += (char)((uint8_t)(msg_bits >> 32));
  buffer += (char)((uint8_t)(msg_bits >> 24));
  buffer += (char)((uint8_t)(msg_bits >> 16));
  buffer += (char)((uint8_t)(msg_bits >> 8));
  buffer += (char)((uint8_t)msg_bits);
  
  return buffer;
}


/* Given a message, it's tag, and an extension, forges a new message under secret-prefix MAC
   without knowing the secret key used to hash the original message. */
std::pair<string, string> forge_MAC(const string & msg, const string & mac, const string & extension,
				    const size_t & key_len){
  string new_msg = "";
  string new_mac = "";
  
  new_msg += msg + pad(msg.size() + key_len) + extension;

  // Slice the mac to get the values with which to fixate the registers in SHA1
  // This would be better as an array
  const uint32_t val1 = hexstr_to_val(mac.substr(0, 8));
  const uint32_t val2 = hexstr_to_val(mac.substr(8, 8));
  const uint32_t val3 = hexstr_to_val(mac.substr(16, 8));
  const uint32_t val4 = hexstr_to_val(mac.substr(24, 8));
  const uint32_t val5 = hexstr_to_val(mac.substr(32, 8));

  const string start_buf = msg + pad(msg.size() + key_len);
  const uint64_t start_transforms = (key_len + start_buf.size())/BLOCK_SIZE_BYTES;

  SHA1 checksum(val1, val2, val3, val4, val5, "", start_transforms);
  checksum.update(extension);
  new_mac = checksum.final();
  
  return std::make_pair(new_msg, new_mac);
}


int main(int /* argc */, const char ** /* argv */)
{
  const string key = "audacious";
  const string msg = "abc";
  const string mac = secret_prefix_MAC(key, msg);
  const string extension = ";admin=true";
  const string goal = msg + pad(msg.size() + key.size()) + extension;
    
  std::pair<string, string> result = forge_MAC(msg, mac, extension, key.size());

  cout << result.second << endl << secret_prefix_MAC(key, goal) << endl;
  
  return 0;
}
