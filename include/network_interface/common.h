/*
* Unpublished Copyright (c) 2009-2019 AutonomouStuff, LLC, All Rights Reserved.
*
* This file is part of the network_interface ROS driver which is released under the MIT license.
* See file LICENSE included with this software or go to https://opensource.org/licenses/MIT for full license details.
*/

#ifndef NETWORK_INTERFACE_COMMON_H
#define NETWORK_INTERFACE_COMMON_H

namespace AS
{
namespace Network
{

enum class ReturnStatuses
{
  OK = 0,
  INIT_FAILED = -1,
  BAD_PARAM = -2,
  SOCKET_ERROR = -3,
  SOCKET_CLOSED = -4,
  NO_MESSAGES_RECEIVED = -5,
  READ_FAILED = -6,
  WRITE_FAILED = -7,
  CLOSE_FAILED = -8,
  SOCKET_TIMEOUT = -9
};

enum class ByteOrder
{
  BE = 0,
  LE
};

}  // namespace Network
}  // namespace AS

#endif  // NETWORK_INTERFACE_COMMON_H
