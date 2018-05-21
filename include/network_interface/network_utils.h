/*
* Unpublished Copyright (c) 2009-2017 AutonomouStuff, LLC, All Rights Reserved.
*
* This file is part of the network_interface ROS 1.0 driver which is released under the MIT license.
* See file LICENSE included with this software or go to https://opensource.org/licenses/MIT for full license details.
*/

#ifndef NETWORK_INTERFACE_NETWORK_UTILS_H
#define NETWORK_INTERFACE_NETWORK_UTILS_H

#include <cstddef>
#include <cstdint>
#include <vector>
#include <typeinfo>
#include <cstring>

namespace AS
{
namespace Network
{
enum ByteOrder
{
  BE = 0,
  LE
};

inline bool system_is_big_endian()
{
  union
  {
    uint32_t i;
    char c[4];
  } big_int = {0x12345678};

  return big_int.c[0] == 1;
}

// little-endian
template<typename T>
T read_le(uint8_t* bufArray,
          const uint32_t& size,
          const uint32_t& offset,
          const float& factor,
          const uint32_t& valueOffset)
{
  uint64_t rcvData = 0;

  for (uint32_t i = size; i > 0; i--)
  {
    rcvData <<= 8;
    // Need to use -1 because array is 0-based
    // and offset is not.
    rcvData |= bufArray[(offset - 1) + i];
  }

  T retVal = 0;
  std::memcpy(&retVal, (system_is_big_endian()) ? &rcvData + sizeof(uint64_t) - sizeof(T) : &rcvData, sizeof(T));
  retVal *= (T) factor;
  retVal += valueOffset;

  return retVal;
};

template<typename T>
T read_le(unsigned char* bufArray,
          const unsigned int& size,
          const unsigned int& offset)
{
  return read_le<T>(bufArray, size, offset, 1.0, 0);
};

template<typename T>
std::vector<uint8_t> write_le(T *source)
{
  std::vector<uint8_t> ret_val;

  if (sizeof(source))
    return ret_val;

  if (typeid(source) == typeid(float) || typeid(source) == typeid(double) || typeid(source) == typeid(long double))  // NOLINT
    return ret_val;

  T mask = 0xFF;

  while ((*source & mask) > 0)
  {
    ret_val.push_back(uint8_t(*source & mask));
    mask <<= 8;
  }

  return ret_val;
};

// big-endian
template<typename T>
T read_be(unsigned char* bufArray,
          const unsigned int& size,
          const unsigned int& offset,
          const float& factor,
          const unsigned int& valueOffset)
{
  uint64_t rcvData = 0;

  for (unsigned int i = 0; i <  size; i++)
  {
    rcvData <<= 8;
    rcvData |= bufArray[(offset) + i];
  }

  T retVal;
  std::memcpy(&retVal, (system_is_big_endian()) ? &rcvData + sizeof(uint64_t) - sizeof(T) : &rcvData, sizeof(T));
  retVal *= (T) factor;
  retVal += valueOffset;

  return retVal;
};

template<typename T>
T read_be(unsigned char* bufArray,
          const unsigned int& size,
          const unsigned int& offset)
{
  return read_be<T>(bufArray, size, offset, 1.0, 0);
}

template<typename T>
std::vector<uint8_t> write_be(T *source)
{
  std::vector<uint8_t> ret_val;

  if (typeid(source) == typeid(float) || typeid(source) == typeid(double) || typeid(source) == typeid(long double))  // NOLINT
  {
    return ret_val;
  }

  T mask = 0xFF;

  int shift = 8 * (sizeof(T) - 1);
  mask <<= shift;

  while (mask > 0)
  {
    // //printf("mask: 0x%016x\n",mask);
    ret_val.push_back(uint8_t(((*source) & mask) >> shift));
    shift -= 8;
    mask >>= 8;
  }

  return ret_val;
};

inline int32_t find_magic_word(uint8_t *in, uint32_t buf_size, size_t magic_word)
{
  bool packet_found = false;
  uint32_t i = 0;
  uint32_t chunk;
  const uint32_t chunk_bytes = 4;

  while (!packet_found && buf_size >= chunk_bytes)
  {
    chunk = read_be<uint32_t>(in, chunk_bytes, i);

    if (chunk == magic_word)
    {
      packet_found = true;
      return i;
    }

    i++;
    buf_size--;
  }

  // Just in case
  return -1;
};
}  // namespace Network
}  // namespace AS

#endif  // NETWORK_INTERFACE_NETWORK_UTILS_H
