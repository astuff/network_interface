/*
* Unpublished Copyright (c) 2009-2019 AutonomouStuff, LLC, All Rights Reserved.
*
* This file is part of the network_interface ROS driver which is released under the MIT license.
* See file LICENSE included with this software or go to https://opensource.org/licenses/MIT for full license details.
*/

#include <network_interface/network_interface.h>
#include <string>

std::string AS::Network::return_status_desc(const ReturnStatuses &ret)
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
