/*
* Unpublished Copyright (c) 2009-2017 AutonomouStuff, LLC, All Rights Reserved.
*
* This file is part of the network_interface ROS 1.0 driver which is released under the MIT license.
* See file LICENSE included with this software or go to https://opensource.org/licenses/MIT for full license details.
*/

#ifndef NETWORK_INTERFACE_NETWORK_UTILS_H
#define NETWORK_INTERFACE_NETWORK_UTILS_H

#include "network_interface/common.h"

#include <cstdint>
#include <vector>
#include <deque>
#include <typeinfo>
#include <string>
#include <cstring>
#include <type_traits>

namespace AS
{
namespace Network
{

// little-endian
template<class T, class C>
T read_le(const C& bufArray,
          const uint32_t& offset,
          const float& factor,
          const float& valueOffset)
{
  uint64_t rcvData = 0;

  for (uint32_t i = sizeof(T); i > 0; i--)
  {
    rcvData <<= 8;
    // Need to use -1 because array is 0-based
    // and offset is not.
    rcvData |= bufArray[(offset - 1) + i];
  }

  T* retVal;

  // Reinterpret the uint64_t as the provided type
  retVal = reinterpret_cast<T*>(&rcvData);
  *retVal *= static_cast<T>(factor);
  *retVal += static_cast<T>(valueOffset);

  return *retVal;
};

template<class T, class C>
T read_le(const C& bufArray,
          const uint32_t& offset)
{
  // Call the other signature of this function
  // with a factor of 1.0 and an offset of 0.
  return read_le<T>(bufArray, offset, 1.0, 0);
};

template<class T>
std::vector<uint8_t> write_le(T *source,
    typename std::enable_if<std::is_integral<T>::value>::type* = 0)
{
  std::vector<uint8_t> ret_val;

  T mask = 0xFF;
  uint32_t shift = 0;

  // Shift is the bitshift value to apply.
  // It starts at 0 and increments by 8 bits
  // for each byte.
  while (shift < (8 * sizeof(T)))
  {
    ret_val.push_back(static_cast<uint8_t>((*source & mask) >> shift));
    shift += 8;
    mask <<= 8;
  }

  return ret_val;
};

template<class T>
std::vector<uint8_t> write_le(T *source,
    typename std::enable_if<std::is_floating_point<T>::value>::type* = 0)
{
  // This function handles floating-point values
  // by finding an unsigned int of the same size
  // and casting the raw bits of the floating-point
  // value to an unsigned int, then calling this
  // function recursively with that value.
  if (sizeof(T) == sizeof(uint16_t))
  {
    return write_le<uint16_t>(
      static_cast<uint16_t*>(
        static_cast<void*>(source)));
  }
  else if (sizeof(T) == sizeof(uint32_t))
  {
    return write_le<uint32_t>(
      static_cast<uint32_t*>(
        static_cast<void*>(source)));
  }
  else if (sizeof(T) == sizeof(uint64_t))
  {
    return write_le<uint64_t>(
      static_cast<uint64_t*>(
        static_cast<void*>(source)));
  }
};

// big-endian
template<class T, class C>
T read_be(const C& bufArray,
          const uint32_t& offset,
          const float& factor,
          const float& valueOffset)
{
  uint64_t rcvData = 0;

  for (uint32_t i = 0; i < sizeof(T); i++)
  {
    rcvData <<= 8;
    // Since we're counting up from 0 to size(T)
    // we don't need the -1 here.
    rcvData |= bufArray[(offset) + i];
  }

  T* retVal;

  // Reinterpret the uint64_t as the provided type
  retVal = reinterpret_cast<T*>(&rcvData);
  *retVal *= static_cast<T>(factor);
  *retVal += static_cast<T>(valueOffset);

  return *retVal;
};

template<class T, class C>
T read_be(const C& bufArray,
          const uint32_t& offset)
{
  // Call the other signature of this function
  // with a factor of 1.0 and an offset of 0.
  return read_be<T>(bufArray, offset, 1.0, 0);
}

template<class T>
std::vector<uint8_t> write_be(T *source,
    typename std::enable_if<std::is_integral<T>::value>::type* = 0)
{
  std::vector<uint8_t> ret_val;

  T mask = 0xFF;

  int shift = 8 * (sizeof(T) - 1);
  mask <<= shift;

  // Shift is the bitshift value to apply.
  // It starts at 8 * number of bytes of T - 1
  // and decrements by 8 bits for each byte.
  while (mask > 0)
  {
    ret_val.push_back(static_cast<uint8_t>((*source & mask) >> shift));
    shift -= 8;
    mask >>= 8;
  }

  return ret_val;
};

template<class T>
std::vector<uint8_t> write_be(T *source,
    typename std::enable_if<std::is_floating_point<T>::value>::type* = 0)
{
  // This function handles floating-point values
  // by finding an unsigned int of the same size
  // and casting the raw bits of the floating-point
  // value to an unsigned int, then calling this
  // function recursively with that value.
  if (sizeof(T) == sizeof(uint16_t))
  {
    return write_be(
      static_cast<uint16_t*>(
        static_cast<void*>(source)));
  }
  else if (sizeof(T) == sizeof(uint32_t))
  {
    return write_be(
      static_cast<uint32_t*>(
        static_cast<void*>(source)));
  }
  else if (sizeof(T) == sizeof(uint64_t))
  {
    return write_be(
      static_cast<uint64_t*>(
        static_cast<void*>(source)));
  }
}

template<typename C>
inline int32_t find_magic_word(const C& in, const size_t& magic_word)
{
  bool packet_found = false;
  uint32_t i = 0;
  uint32_t chunk = 0;
  auto buf_size = in.size();

  // Read 4 bytes at a time
  while (!packet_found && buf_size >= 4)
  {
    chunk = read_be<uint32_t>(in, i);

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

template<class T, class C>
void parse_tuple(
  const C& in,
  T *out1,
  T *out2,
  ByteOrder bo,
  const uint16_t& offset)
{
  if (bo == ByteOrder::LE)
  {
    *out1 = read_le<T>(in, offset);
    *out2 = read_le<T>(in, offset + sizeof(T));
  }
  else if (bo == ByteOrder::BE)
  {
    *out1 = read_be<T>(in, offset);
    *out2 = read_be<T>(in, offset + sizeof(T));
  }
};

}  // namespace Network
}  // namespace AS

#endif  // NETWORK_INTERFACE_NETWORK_UTILS_H
