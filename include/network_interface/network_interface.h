/*
*   network_interface.h - Header file for AutonomouStuff generic network interface.
*   Copyright (C) 2017 AutonomouStuff, Co.
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Define a class that supports a basic network interface that's independent of the hardware and driver library used
// Different libraries can be created to define all these functions for a specific driver library

#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

//C++ Includes
#include <cstdio>
#include <iostream>
#include <boost/asio.hpp>

//OS Includes
#include <unistd.h>

namespace AS
{
namespace Network
{
  enum return_statuses
  {
    OK = 0,
    INIT_FAILED = -1,
    BAD_PARAM = -2,
    SOCKET_ERROR = -3,
    SOCKET_CLOSED = -4,
    NO_MESSAGES_RECEIVED = -5,
    READ_FAILED = -6,
    WRITE_FAILED = -7,
    CLOSE_FAILED = -8
  };
  
  class UDPInterface
  {
    public:
      UDPInterface();

      ~UDPInterface();

      // Called to pass in parameters and open ethernet link
      return_statuses open(const char *ip_address, const int &port);

      // Close the ethernet link
      return_statuses close();

      // Check on the status of the link
      bool is_open();

      // Read a message - UDP is datagram-based so you cannot read exactly x bytes.
      return_statuses read(unsigned char *msg,
                           const size_t &buf_size,
                           size_t &bytes_read);

      // Send a message
      return_statuses write(unsigned char *msg, const size_t &msg_size);

    private:
      boost::asio::io_service io_service_;
      boost::asio::ip::udp::socket socket_;
      boost::asio::ip::udp::endpoint sender_endpoint_;
  };
  
  class TCPInterface
  {
    public:
      TCPInterface();
                 
      ~TCPInterface();
          
      // Called to pass in parameters and open ethernet link
      return_statuses open(const char *ip_address,
                           const int &port);

      // Close the ethernet link
      return_statuses close();

      // Check on the status of the link
      bool is_open();

      // Read a message
      return_statuses read(unsigned char *msg,
                           const size_t &buf_size,
                           size_t &bytes_read);
      return_statuses read_exactly(unsigned char *msg,
                                   const size_t &buf_size,
                                   const size_t &bytes_to_read);

      // Send a message
      return_statuses write(unsigned char *msg, const size_t &msg_size);
    private:
      boost::asio::io_service io_service_;
      boost::asio::ip::tcp::socket socket_;
  };
  
  //Utility Functions
  std::string return_status_desc(const return_statuses &ret);
}  
}
#endif
