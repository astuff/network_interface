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
#include <iostream>
#include <boost/asio.hpp>

//OS Includes
#include <unistd.h>
#include <netinet/in.h>

namespace AS
{
namespace Network
{
  enum return_statuses
  {
    ok = 0,
    init_failed,
    no_messages_received,
    send_failed
  };
  
  class UDPInterface
  {
    public:
      UDPInterface();

      ~UDPInterface();

      // Called to pass in parameters and open ethernet link
      return_statuses open(const char *local_address,
                            const char *remote_address,
                            int local_port,
                            int remote_port);

      // Close the ethernet link
      return_statuses close();

      // Read a message
      return_statuses read(unsigned char *msg, unsigned int *size, unsigned int buf_size);

      // Send a message
      return_statuses send(unsigned char *msg, unsigned int size);

    private:
      int socket_;
      in_addr_t remote_addr_;
      int remote_port_;
      bool ok_;
  };
  
  class TCPInterface
  {
    public:
      TCPInterface();
                 
      ~TCPInterface();
          
      // Called to pass in parameters and open ethernet link
      return_statuses open(const char *ip_address, int port);
      // Close the ethernet link
      return_statuses close();
      // Read a message
      return_statuses read_some(unsigned char *msg, size_t buf_size, size_t &bytes_read);
      return_statuses read_exactly(unsigned char *msg, size_t buf_size, unsigned int bytes);
      // Send a message
      return_statuses send(unsigned char *msg, size_t size);
    private:
      boost::asio::io_service io_service_;
      boost::asio::ip::tcp::socket socket_;
      bool ok_;
  };
}  
}
#endif
