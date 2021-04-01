/*
* Unpublished Copyright (c) 2009-2021 AutonomouStuff, LLC, All Rights Reserved.
*
* This file is part of the network_interface ROS 1.0 driver which is released under the MIT license.
* See file LICENSE included with this software or go to https://opensource.org/licenses/MIT for full license details.
*/

// Define a class that supports a basic network interface that's independent of the hardware and driver library used
// Different libraries can be created to define all these functions for a specific driver library

#ifndef NETWORK_INTERFACE_UDP_SERVER_H
#define NETWORK_INTERFACE_UDP_SERVER_H

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

class UDPServer
{
public:
  UDPServer();

  // Called to pass in parameters and open ethernet link
  ReturnStatuses open(const std::string& ip_address, uint32_t port);

  // Close the bound socket connection
  ReturnStatuses close();

  // Check on the status of the link
  bool is_open();

  // Register a callback for receiving data and sending a response
  void registerReceiveHandler(std::function<std::vector<uint8_t>(const std::vector<uint8_t>&)> callback);

  // Run the io_service, will block until shutdown
  void run();

  // Stop the io_service, thread safe
  void stop();

private:
  // Queue an io_service task for receiving on the socket
  void startReceive();

  // Callback for when received data is ready
  void handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);

  // Callback for when server replies are sent
  void handleSend(std::vector<uint8_t> sent_payload,
    const boost::system::error_code& ec, std::size_t bytes_transferred);

  boost::asio::io_service io_service_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint server_endpoint_;
  boost::asio::ip::udp::endpoint client_endpoint_;
  std::function<std::vector<uint8_t>(const std::vector<uint8_t>&)> receive_callback_;
};

std::string return_status_desc(const ReturnStatuses &ret);

}  // namespace Network
}  // namespace AS
#endif  // NETWORK_INTERFACE_UDP_SERVER_H
