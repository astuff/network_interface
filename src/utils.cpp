/*
* Unpublished Copyright (c) 2009-2017 AutonomouStuff, LLC, All Rights Reserved.
*
* This file is part of the network_interface ROS 1.0 driver which is released under the MIT license.
* See file LICENSE included with this software or go to https://opensource.org/licenses/MIT for full license details.
*/

#include <string>
#include <network_interface.h>

std::string AS::Network::return_status_desc(const return_statuses &ret)
{
  std::string status_string;

  if (ret == INIT_FAILED)
  {
    status_string = "Initialization of the network interface failed.";
  }
  else if (ret == BAD_PARAM)
  {
    status_string = "A bad parameter was provided to the network interface during initalization.";
  }
  else if (ret == SOCKET_ERROR)
  {
    status_string = "A socket error was encountered.";
  }
  else if (ret == SOCKET_CLOSED)
  {
    status_string = "Socket is not currently open.";
  }
  else if (ret == NO_MESSAGES_RECEIVED)
  {
    status_string = "No messages were received on the interface.";
  }
  else if (ret == READ_FAILED)
  {
    status_string = "A read operation failed on the network interface.";
  }
  else if (ret == WRITE_FAILED)
  {
    status_string = "A write operation failed on the network interface.";
  }
  else if (ret == CLOSE_FAILED)
  {
    status_string = "Closing the network failed.";
  }

  return status_string;
}
