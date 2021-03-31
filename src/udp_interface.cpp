/*
* Unpublished Copyright (c) 2009-2017 AutonomouStuff, LLC, All Rights Reserved.
*
* This file is part of the network_interface ROS 1.0 driver which is released under the MIT license.
* See file LICENSE included with this software or go to https://opensource.org/licenses/MIT for full license details.
*/

#include <network_interface/network_interface.h>

#include <string>
#include <vector>

using namespace AS::Network;  // NOLINT
using boost::asio::ip::udp;

// Default constructor.
UDPInterface::UDPInterface() :
  socket_(io_service_)
{
}

ReturnStatuses UDPInterface::open(const std::string& ip_address, uint32_t port, uint32_t receive_buffer_size)
{
  receive_buffer_size_ = receive_buffer_size;
  return UDPInterface::open(ip_address, port);
}

ReturnStatuses UDPInterface::open(const std::string& ip_address, uint32_t port)
{
  if (socket_.is_open())
    return ReturnStatuses::OK;

  std::stringstream sPort;
  sPort << port;
  udp::resolver res(io_service_);
  udp::resolver::query query(udp::v4(), ip_address.c_str(), sPort.str());
  sender_endpoint_ = *res.resolve(query);
  boost::system::error_code ec;

  socket_.connect(sender_endpoint_, ec);

  if (ec.value() == boost::system::errc::success)
  {
    return ReturnStatuses::OK;
  }
  else if (ec.value() == boost::asio::error::invalid_argument)
  {
    return ReturnStatuses::BAD_PARAM;
  }
  else
  {
    close();
    return ReturnStatuses::INIT_FAILED;
  }
}

ReturnStatuses UDPInterface::close()
{
  if (!socket_.is_open())
    return ReturnStatuses::SOCKET_CLOSED;

  boost::system::error_code ec;
  socket_.close(ec);

  if (ec.value() == boost::system::errc::success)
    return ReturnStatuses::OK;
  else
    return ReturnStatuses::CLOSE_FAILED;
}

bool UDPInterface::is_open()
{
  return socket_.is_open();
}

ReturnStatuses UDPInterface::read(std::vector<uint8_t> *msg)
{
  if (!socket_.is_open())
    return ReturnStatuses::SOCKET_CLOSED;

  boost::system::error_code ec;
  msg->assign(receive_buffer_size_, 0);
  size_t msg_size = socket_.receive(boost::asio::buffer(*msg), 0, ec);
  msg->resize(msg_size);

  if (ec.value() == boost::system::errc::success)
    return ReturnStatuses::OK;
  else
    return ReturnStatuses::READ_FAILED;
}

ReturnStatuses UDPInterface::write(const std::vector<uint8_t>& msg)
{
  if (!socket_.is_open())
    return ReturnStatuses::SOCKET_CLOSED;

  boost::system::error_code ec;
  socket_.send_to(boost::asio::buffer(msg), sender_endpoint_, 0, ec);

  if (ec.value() == boost::system::errc::success)
    return ReturnStatuses::OK;
  else
    return ReturnStatuses::WRITE_FAILED;
}
