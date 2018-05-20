/*
* Unpublished Copyright (c) 2009-2017 AutonomouStuff, LLC, All Rights Reserved.
*
* This file is part of the network_interface ROS 1.0 driver which is released under the MIT license.
* See file LICENSE included with this software or go to https://opensource.org/licenses/MIT for full license details.
*/

// Define a class that supports a basic network interface that's independent of the hardware and driver library used
// Different libraries can be created to define all these functions for a specific driver library

#ifndef NETWORK_INTERFACE_NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_NETWORK_INTERFACE_H

// C++ Includes
#include <cstdio>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

// OS Includes
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
  CLOSE_FAILED = -8,
  SOCKET_TIMEOUT = -9
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
                       size_t &bytes_read,
                       int timeout_ms = 0);  // Optional timeout argument, in milliseconds
  return_statuses read_exactly(unsigned char *msg,
                               const size_t &buf_size,
                               const size_t &bytes_to_read,
                               int timeout_ms = 0);  // Optional timeout argument, in milliseconds

  // Send a message
  return_statuses write(unsigned char *msg, const size_t &msg_size);
private:
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::socket socket_;
  boost::system::error_code error_;
  size_t bytes_read_;

  void timeout_handler(const boost::system::error_code& error);
  void read_handler(const boost::system::error_code& error, size_t bytes_read);
};

// Utility Functions
std::string return_status_desc(const return_statuses &ret);
}  // namespace Network
}  // namespace AS
#endif  // NETWORK_INTERFACE_NETWORK_INTERFACE_H
