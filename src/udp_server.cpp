/*
* Unpublished Copyright (c) 2009-2021 AutonomouStuff, LLC, All Rights Reserved.
*
* This file is part of the network_interface ROS 1.0 driver which is released under the MIT license.
* See file LICENSE included with this software or go to https://opensource.org/licenses/MIT for full license details.
*/

#include <network_interface/udp_server.h>

#include <string>
#include <vector>

using namespace AS::Network;  // NOLINT
using boost::asio::ip::udp;

UDPServer::UDPServer() :
  socket_(io_service_)
{
}

ReturnStatuses UDPServer::open(const std::string& ip_address, uint32_t port)
{
  if (socket_.is_open())
  {
    return ReturnStatuses::OK;
  }

  std::stringstream sPort;
  sPort << port;
  udp::resolver res(io_service_);
  udp::resolver::query query(udp::v4(), ip_address.c_str(), sPort.str());
  server_endpoint_ = *res.resolve(query);
  boost::system::error_code ec;

  socket_.open(boost::asio::ip::udp::v4(), ec);
  socket_.bind(server_endpoint_, ec);

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

ReturnStatuses UDPServer::close()
{
  if (!socket_.is_open())
  {
    return ReturnStatuses::SOCKET_CLOSED;
  }

  boost::system::error_code ec;
  socket_.close(ec);

  if (ec.value() == boost::system::errc::success)
  {
    return ReturnStatuses::OK;
  }
  else
  {
    return ReturnStatuses::CLOSE_FAILED;
  }
}

bool UDPServer::is_open()
{
  return socket_.is_open();
}

void UDPServer::registerReceiveHandler(std::function<std::vector<uint8_t>(const std::vector<uint8_t>&)> callback)
{
  receive_callback_ = callback;
  startReceive();
}

void UDPServer::startReceive()
{
  socket_.async_receive_from(
    boost::asio::null_buffers(), client_endpoint_,
    boost::bind(&UDPServer::handleReceive, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void UDPServer::handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred)
{
  // unused
  (void)bytes_transferred;

  if (!error || error == boost::asio::error::message_size)
  {
    std::vector<uint8_t> payload;
    boost::system::error_code ec;
    uint32_t available = socket_.available();
    payload.resize(available, 0);

    socket_.receive_from(boost::asio::buffer(payload, available),
      client_endpoint_, 0, ec);

    // User callback
    std::vector<uint8_t> response = receive_callback_(payload);

    // Send response
    if (response.size() > 0)
    {
      socket_.async_send_to(boost::asio::buffer(response), client_endpoint_,
        boost::bind(&UDPServer::handleSend, this, response,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      startReceive();
    }
  }
}

void UDPServer::handleSend(std::vector<uint8_t> sent_payload,
  const boost::system::error_code& ec, std::size_t bytes_transferred)
{
  // unused
  (void)sent_payload;
  (void)ec;
  (void)bytes_transferred;

  startReceive();
}

void UDPServer::run()
{
  io_service_.run();
}

void UDPServer::stop()
{
  io_service_.stop();
}
