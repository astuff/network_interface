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
T read_le(const std::vector<uint8_t>& bufArray,
          const uint32_t& offset,
          const float& factor,
          const uint32_t& valueOffset)
{
  uint64_t rcvData = 0;

  for (uint32_t i = sizeof(T); i > 0; i--)
  {
    rcvData <<= 8;
    // Need to use -1 because array is 0-based
    // and offset is not.
    rcvData |= bufArray[(offset - 1) + i];
  }

  T retVal = 0;

  if (system_is_big_endian())
    std::memcpy(&retVal, &rcvData + sizeof(uint64_t) - sizeof(T), sizeof(T));
  else
    std::memcpy(&retVal, &rcvData, sizeof(T));

  retVal *= static_cast<T>(factor);
  retVal += valueOffset;

  return retVal;
};

template<typename T>
T read_le(const std::vector<uint8_t>& bufArray,
          const unsigned int& offset)
{
  return read_le<T>(bufArray, offset, 1.0, 0);
};

template<typename T>
std::vector<uint8_t> write_le(T *source)
{
  std::vector<uint8_t> ret_val;

  if (std::is_floating_point<T>::type)
    return ret_val;

  T mask = 0xFF;

  while ((*source & mask) > 0)
  {
    ret_val.push_back(static_cast<uint8_t>(*source & mask));
    mask <<= 8;
  }

  return ret_val;
};

// big-endian
template<typename T>
T read_be(const std::vector<uint8_t>& bufArray,
          const unsigned int& offset,
          const float& factor,
          const unsigned int& valueOffset)
{
  uint64_t rcvData = 0;

  for (unsigned int i = 0; i < sizeof(T); i++)
  {
    rcvData <<= 8;
    rcvData |= bufArray[(offset) + i];
  }

  T retVal;

  if (system_is_big_endian())
    std::memcpy(&retVal, &rcvData + sizeof(uint64_t) - sizeof(T), sizeof(T));
  else
    std::memcpy(&retVal, &rcvData, sizeof(T));

  retVal *= (T) factor;
  retVal += valueOffset;

  return retVal;
};

template<typename T>
T read_be(const std::vector<uint8_t>& bufArray,
          const unsigned int& offset)
{
  return read_be<T>(bufArray, offset, 1.0, 0);
}

template<typename T>
std::vector<uint8_t> write_be(T *source)
{
  std::vector<uint8_t> ret_val;

  if (std::is_floating_point<T>::type)
    return ret_val;

  T mask = 0xFF;

  int shift = 8 * (sizeof(T) - 1);
  mask <<= shift;

  while (mask > 0)
  {
    // //printf("mask: 0x%016x\n",mask);
    ret_val.push_back(static_cast<uint8_t>((*source & mask) >> shift));
    shift -= 8;
    mask >>= 8;
  }

  return ret_val;
};

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
