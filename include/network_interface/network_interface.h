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

#include "network_interface/common.h"

// C++ Includes
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

// OS Includes
#include <unistd.h>

namespace AS
{
namespace Network
{

class UDPInterface
{
public:
  UDPInterface();

  // Called to pass in parameters and open ethernet link
  ReturnStatuses open(const std::string& ip_address, uint32_t port, uint32_t receive_buffer_size);
  ReturnStatuses open(const std::string& ip_address, uint32_t port);

  // Close the ethernet link
  ReturnStatuses close();

  // Check on the status of the link
  bool is_open();

  // Read a message - UDP is datagram-based so you cannot read exactly x bytes.
  ReturnStatuses read(std::vector<uint8_t> *msg);

  // Send a message
  ReturnStatuses write(const std::vector<uint8_t>& msg);

private:
  uint32_t receive_buffer_size_ = 1024;
  boost::asio::io_service io_service_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint sender_endpoint_;
};

class TCPInterface
{
public:
  TCPInterface();

  // Called to pass in parameters and open ethernet link
  ReturnStatuses open(std::string ip_address, const int &port);

  // Close the ethernet link
  ReturnStatuses close();

  // Check on the status of the link
  bool is_open();

  // Read a message
  ReturnStatuses read(std::vector<uint8_t> *msg);
  ReturnStatuses read_exactly(std::vector<uint8_t> *msg, const size_t &bytes_to_read);

  // Send a message
  ReturnStatuses write(const std::vector<uint8_t> &msg);
private:
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::socket socket_;
  boost::system::error_code error_;
};

std::string return_status_desc(const ReturnStatuses &ret);

}  // namespace Network
}  // namespace AS
#endif  // NETWORK_INTERFACE_NETWORK_INTERFACE_H
