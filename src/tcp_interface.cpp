/*
*   tcp_interface.cpp - TCP implementation for AutonomouStuff generic network interface.
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

#include <network_interface.h>

using namespace std;
using namespace AS::Network;
using boost::asio::ip::tcp;

//Default constructor.
TCPInterface::TCPInterface() :
  socket_(io_service_)
{
}

//Default destructor.
TCPInterface::~TCPInterface()
{
}

return_statuses TCPInterface::open(const char *ip_address, const int &port)
{
  if (socket_.is_open())
    return OK;

  stringstream sPort;
  sPort << port;
  tcp::resolver res(io_service_);
  tcp::resolver::query query(tcp::v4(), ip_address, sPort.str());
  tcp::resolver::iterator it = res.resolve(query);
  boost::system::error_code ec;

  socket_.connect(*it, ec);

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

return_statuses TCPInterface::close()
{
  if (!socket_.is_open())
    return OK;

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

bool TCPInterface::is_open()
{
  return socket_.is_open();
}

return_statuses TCPInterface::read(unsigned char *msg,
                                   const size_t &buf_size,
                                   size_t &bytes_read)
{
  if (!socket_.is_open())
    return SOCKET_CLOSED;

  boost::system::error_code ec;
  bytes_read = boost::asio::read(socket_, boost::asio::buffer(msg, buf_size), ec);

  if (ec.value() == boost::system::errc::success)
  {
    return OK;
  }
  else
  {
    return READ_FAILED;
  }
}

return_statuses TCPInterface::read_exactly(unsigned char *msg,
                                           const size_t &buf_size,
                                           const size_t &bytes_to_read)
{
  if (!socket_.is_open())
    return SOCKET_CLOSED;

  boost::system::error_code ec;
  boost::asio::read(socket_, boost::asio::buffer(msg, buf_size), boost::asio::transfer_exactly(bytes_to_read), ec);

  if (ec.value() == boost::system::errc::success)
  {
    return OK;
  }
  else
  {
    return READ_FAILED;
  }
}

return_statuses TCPInterface::write(unsigned char *msg, const size_t &msg_size)
{
  if (!socket_.is_open())
    return SOCKET_CLOSED;

  boost::system::error_code ec;
  boost::asio::write(socket_, boost::asio::buffer(msg, msg_size), ec);

  if (ec.value() == boost::system::errc::success)
  {
    return OK;
  }
  else
  {
    return WRITE_FAILED;
  }
}
