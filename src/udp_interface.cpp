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

// Default destructor.
UDPInterface::~UDPInterface()
{
}

return_statuses UDPInterface::open(std::string ip_address, const int &port)
{
  if (socket_.is_open())
    return OK;

  std::stringstream sPort;
  sPort << port;
  udp::resolver res(io_service_);
  udp::resolver::query query(udp::v4(), ip_address.c_str(), sPort.str());
  sender_endpoint_ = *res.resolve(query);
  boost::system::error_code ec;

  socket_.connect(sender_endpoint_, ec);

  if (ec.value() == boost::system::errc::success)
  {
    return OK;
  }
  else if (ec.value() == boost::asio::error::invalid_argument)
  {
    return BAD_PARAM;
  }
  else
  {
    close();
    return INIT_FAILED;
  }
}

return_statuses UDPInterface::close()
{
  if (!socket_.is_open())
    return SOCKET_CLOSED;

  boost::system::error_code ec;
  socket_.close(ec);

  if (ec.value() == boost::system::errc::success)
  {
    return OK;
  }
  else
  {
    return CLOSE_FAILED;
  }
}

bool UDPInterface::is_open()
{
  return socket_.is_open();
}

return_statuses UDPInterface::read(std::vector<uint8_t> *msg, size_t *bytes_read)
{
  if (!socket_.is_open())
    return SOCKET_CLOSED;

  boost::system::error_code ec;
  msg->assign(10000, 0);
  *bytes_read = socket_.receive_from(boost::asio::buffer(*msg), sender_endpoint_, 0, ec);

  if (ec.value() == boost::system::errc::success)
  {
    return OK;
  }
  else
  {
    return READ_FAILED;
  }
}

return_statuses UDPInterface::write(const std::vector<uint8_t> &msg)
{
  if (!socket_.is_open())
    return SOCKET_CLOSED;

  boost::system::error_code ec;
  socket_.send_to(boost::asio::buffer(msg), sender_endpoint_, 0, ec);

  if (ec.value() == boost::system::errc::success)
  {
    return OK;
  }
  else
  {
    return WRITE_FAILED;
  }
}
