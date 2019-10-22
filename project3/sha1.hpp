/*
  sha1.hpp - header of
  
  ============
  SHA-1 in C++
  ============
  
  100% Public Domain.
  
  Original C Code
  -- Steve Reid <steve@edmweb.com>
  Small changes to fit into bglibs
  -- Bruce Guenter <bruce@untroubled.org>
  Translation to simpler C++ Code
  -- Volker Diels-Grabsch <v@njh.eu>
  Safety fixes
  -- Eugene Hopkinson <slowriot at voxelstorm dot com>
*/

#ifndef SHA1_HPP
#define SHA1_HPP


#include <cstdint>
#include <iostream>
#include <string>


class SHA1
{
public:
  SHA1();
  SHA1(const uint32_t val1, const uint32_t val2, const uint32_t val3,
       const uint32_t val4, const uint32_t val5, const std::string start_buf,
       const uint64_t start_transforms);
  void update(const std::string &s);
  void update(std::istream &is);
  std::string final();
  static std::string from_file(const std::string &filename);
  
private:
  uint32_t _digest[5];
  std::string _buffer;
  uint64_t _transforms;
};


#endif /* SHA1_HPP */
