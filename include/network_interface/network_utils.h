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
#include <typeinfo>
#include <string>
#include <cstring>
#include <type_traits>

namespace AS
{
namespace Network
{

// little-endian
template<typename T>
T read_le(const std::vector<uint8_t>& bufArray,
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

  retVal = reinterpret_cast<T*>(&rcvData);
  *retVal *= static_cast<T>(factor);
  *retVal += static_cast<T>(valueOffset);

  return *retVal;
};

template<typename T>
T read_le(const std::vector<uint8_t>& bufArray,
          const uint32_t& offset)
{
  return read_le<T>(bufArray, offset, 1.0, 0);
};

template<typename T>
std::vector<uint8_t> write_le(T *source,
    typename std::enable_if<std::is_integral<T>::value>::type* = 0)
{
  std::vector<uint8_t> ret_val;

  T mask = 0xFF;
  uint32_t shift = 0;

  while (shift < (8 * sizeof(T)))
  {
    ret_val.push_back(static_cast<uint8_t>((*source & mask) >> shift));
    shift += 8;
    mask <<= 8;
  }

  return ret_val;
};

template<typename T>
std::vector<uint8_t> write_le(T *source,
    typename std::enable_if<std::is_floating_point<T>::value>::type* = 0)
{
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
template<typename T>
T read_be(const std::vector<uint8_t>& bufArray,
          const uint32_t& offset,
          const float& factor,
          const float& valueOffset)
{
  uint64_t rcvData = 0;

  for (uint32_t i = 0; i < sizeof(T); i++)
  {
    rcvData <<= 8;
    rcvData |= bufArray[(offset) + i];
  }

  T* retVal;

  retVal = reinterpret_cast<T*>(&rcvData);
  *retVal *= static_cast<T>(factor);
  *retVal += static_cast<T>(valueOffset);

  return *retVal;
};

template<typename T>
T read_be(const std::vector<uint8_t>& bufArray,
          const uint32_t& offset)
{
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

inline int32_t find_magic_word(const std::vector<uint8_t>& in, const size_t& magic_word)
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

std::string return_status_desc(const ReturnStatuses &ret)
{
  if (ret == ReturnStatuses::INIT_FAILED)
    return "Initialization of the network interface failed.";
  else if (ret == ReturnStatuses::BAD_PARAM)
    return "A bad parameter was provided to the network interface during initalization.";
  else if (ret == ReturnStatuses::SOCKET_ERROR)
    return "A socket error was encountered.";
  else if (ret == ReturnStatuses::SOCKET_CLOSED)
    return "Socket is not currently open.";
  else if (ret == ReturnStatuses::NO_MESSAGES_RECEIVED)
    return "No messages were received on the interface.";
  else if (ret == ReturnStatuses::READ_FAILED)
    return "A read operation failed on the network interface.";
  else if (ret == ReturnStatuses::WRITE_FAILED)
    return "A write operation failed on the network interface.";
  else if (ret == ReturnStatuses::CLOSE_FAILED)
    return "Closing the network failed.";
  else
    return "Undefined error.";
}

}  // namespace Network
}  // namespace AS

#endif  // NETWORK_INTERFACE_NETWORK_UTILS_H
