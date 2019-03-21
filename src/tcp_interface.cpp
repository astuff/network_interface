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

void TCPInterface::timeout_handler(const boost::system::error_code& error)
{
  // If the operation was not aborted, store the bytes that were read and set the read flag
  if (error != boost::asio::error::operation_aborted)
  {
    error_.assign(boost::system::errc::timed_out, boost::system::system_category());
  }
}

void TCPInterface::read_handler(const boost::system::error_code& error, size_t bytes_read)
{
  bytes_read_ = bytes_read;
}

ReturnStatuses TCPInterface::read(std::vector<uint8_t> *msg,
                                   size_t &bytes_read,
                                   int timeout_ms)
{
  if (!socket_.is_open())
    return ReturnStatuses::SOCKET_CLOSED;

  error_.assign(boost::system::errc::success, boost::system::system_category());

  boost::asio::deadline_timer timer(io_service_);
  // If requested timeout duration is set to 0 ms, don't set a deadline
  if (timeout_ms > 0)
  {
    timer.expires_from_now(boost::posix_time::milliseconds(timeout_ms));
    timer.async_wait(boost::bind(&TCPInterface::timeout_handler,
                                 this,
                                 boost::asio::placeholders::error));
  }

  msg->assign(10000, 0);
  boost::asio::async_read(socket_,
                          boost::asio::buffer(*msg),
                          boost::bind(&TCPInterface::read_handler,
                                      this,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
  // Run until a handler is called
  while (io_service_.run_one())
  {
    if (error_.value() == boost::system::errc::success)
    {
      timer.cancel();
      bytes_read = bytes_read_;
    }
    else if (error_.value() == boost::system::errc::timed_out)
    {
      socket_.cancel();
    }
  }
  // Reset the io service so that it is available for the next call to TCPInterface::read
  io_service_.reset();

  if (error_.value() == boost::system::errc::success)
  {
    return ReturnStatuses::OK;
  }
  else if (error_.value() == boost::system::errc::timed_out)
  {
    return ReturnStatuses::SOCKET_TIMEOUT;
  }
  else
  {
    return ReturnStatuses::READ_FAILED;
  }
}

ReturnStatuses TCPInterface::read_exactly(std::vector<uint8_t> *msg, const size_t &bytes_to_read, int timeout_ms)
{
  if (!socket_.is_open())
    return ReturnStatuses::SOCKET_CLOSED;

  error_.assign(boost::system::errc::success, boost::system::system_category());

  boost::asio::deadline_timer timer(io_service_);
  // If requested timeout duration is set to 0 ms, don't set a deadline
  if (timeout_ms > 0)
  {
    timer.expires_from_now(boost::posix_time::milliseconds(timeout_ms));
    timer.async_wait(boost::bind(&TCPInterface::timeout_handler,
                                 this,
                                 boost::asio::placeholders::error));
  }

  boost::asio::async_read(socket_,
                          boost::asio::buffer(*msg),
                          boost::asio::transfer_exactly(bytes_to_read),
                          boost::bind(&TCPInterface::read_handler,
                                      this,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
  // Run until a handler is called
  while (io_service_.run_one())
  {
    if (error_.value() == boost::system::errc::success)
    {
      timer.cancel();
    }
    else if (error_.value() == boost::system::errc::timed_out)
    {
      socket_.cancel();
    }
  }
  // Reset the io service so that it is available for the next call to TCPInterface::read_exactly
  io_service_.reset();

  if (error_.value() == boost::system::errc::success)
  {
    return ReturnStatuses::OK;
  }
  else if (error_.value() == boost::system::errc::timed_out)
  {
    return ReturnStatuses::SOCKET_TIMEOUT;
  }
  else
  {
    return ReturnStatuses::READ_FAILED;
  }
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
