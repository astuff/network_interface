/*
*   udp_interface.h - UDP implementation for AutonomouStuff generic network interface.
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
using boost::asio::ip::udp;

//Default constructor.
UDPInterface::UDPInterface() :
  socket_(io_service_)
{
}

//Default destructor.
UDPInterface::~UDPInterface()
{
}

return_statuses UDPInterface::open(const char *ip_address, const int &port)
{
  if (socket_.is_open())
    return OK;

  stringstream sPort;
  sPort << port;
  udp::resolver res(io_service_);
  udp::resolver::query query(udp::v4(), ip_address, sPort.str());
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

return_statuses UDPInterface::read(unsigned char *msg,
                                   const size_t &buf_size,
                                   size_t &bytes_read)
{
  if (!socket_.is_open())
    return SOCKET_CLOSED;

  boost::system::error_code ec;
  bytes_read = socket_.receive_from(boost::asio::buffer(msg, buf_size), sender_endpoint_, 0, ec);

  if (ec.value() == boost::system::errc::success)
  {
    return OK;
  }
  else
  {
    return READ_FAILED;
  }
}

return_statuses UDPInterface::write(unsigned char *msg, const size_t &msg_size)
{
  if (!socket_.is_open())
    return SOCKET_CLOSED;

  boost::system::error_code ec;
  socket_.send_to(boost::asio::buffer(msg, msg_size), sender_endpoint_, 0, ec);

  if (ec.value() == boost::system::errc::success)
  {
    return OK;
  }
  else
  {
    return WRITE_FAILED;
  }
}
