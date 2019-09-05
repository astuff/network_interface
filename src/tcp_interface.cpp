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
using boost::asio::ip::tcp;

// Default constructor.
TCPInterface::TCPInterface() :
  socket_(io_service_)
{
}

ReturnStatuses TCPInterface::open(std::string ip_address, const int &port)
{
  if (socket_.is_open())
    return ReturnStatuses::OK;

  std::stringstream sPort;
  sPort << port;
  tcp::resolver res(io_service_);
  tcp::resolver::query query(tcp::v4(), ip_address, sPort.str());
  tcp::resolver::iterator it = res.resolve(query);
  boost::system::error_code ec;

  socket_.connect(*it, ec);

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

ReturnStatuses TCPInterface::close()
{
  if (!socket_.is_open())
    return ReturnStatuses::OK;

  boost::system::error_code ec;
  socket_.close(ec);

  if (ec.value() == boost::system::errc::success)
    return ReturnStatuses::OK;
  else
    return ReturnStatuses::CLOSE_FAILED;
}

bool TCPInterface::is_open()
{
  return socket_.is_open();
}

ReturnStatuses TCPInterface::read(std::vector<uint8_t> *msg)
{
  if (!socket_.is_open())
    return ReturnStatuses::SOCKET_CLOSED;

  error_.assign(boost::system::errc::success, boost::system::system_category());

  msg->resize(socket_.available(), 0);
  boost::asio::read(socket_, boost::asio::buffer(*msg), error_);

  // Reset the io service so that it is available for the next call to TCPInterface::read
  io_service_.reset();

  if (error_.value() == boost::system::errc::success)
    return ReturnStatuses::OK;
  else
    return ReturnStatuses::READ_FAILED;
}

ReturnStatuses TCPInterface::read_exactly(std::vector<uint8_t> *msg, const size_t &bytes_to_read)
{
  if (!socket_.is_open())
    return ReturnStatuses::SOCKET_CLOSED;

  error_.assign(boost::system::errc::success, boost::system::system_category());

  msg->resize(bytes_to_read, 0);
  boost::asio::read(socket_, boost::asio::buffer(*msg), boost::asio::transfer_exactly(bytes_to_read));

  // Reset the io service so that it is available for the next call to TCPInterface::read_exactly
  io_service_.reset();

  if (error_.value() == boost::system::errc::success)
    return ReturnStatuses::OK;
  else
    return ReturnStatuses::READ_FAILED;
}

ReturnStatuses TCPInterface::write(const std::vector<uint8_t> &msg)
{
  if (!socket_.is_open())
    return ReturnStatuses::SOCKET_CLOSED;

  boost::system::error_code ec;
  boost::asio::write(socket_, boost::asio::buffer(msg), ec);

  if (ec.value() == boost::system::errc::success)
  {
    return ReturnStatuses::OK;
  }
  else
  {
    return ReturnStatuses::WRITE_FAILED;
  }
}
