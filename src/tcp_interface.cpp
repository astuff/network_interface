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
#include <sys/socket.h>

#include <cstring>

using namespace AS::Network;
using namespace std;
using namespace boost;
using boost::asio::ip::tcp;

//Default constructor.
TCPInterface::TCPInterface() :
  ok_(false) , socket_(io_service_)
{
}

//Default destructor.
TCPInterface::~TCPInterface()
{
}

return_statuses TCPInterface::open(const char *ip_address, int port)
{
  stringstream sPort;
  sPort << port;
  tcp::resolver res(io_service_);
  tcp::resolver::query query(tcp::v4(), ip_address, sPort.str());
  tcp::resolver::iterator it = res.resolve(query);

  try 
  {
    socket_.connect(*it);
  }
  catch (std::exception& e) {
    close();
    return init_failed;
  }
  
  ok_ = true;
  return ok;
}

return_statuses TCPInterface::close()
{
  if (!ok_)
    return init_failed;

  socket_.close();
  return ok;
}

return_statuses TCPInterface::read_exactly(unsigned char *msg, size_t buf_size, unsigned int bytes)
{
  if (!ok_)
    return init_failed;

  boost::system::error_code ec;
  int rcv_size = asio::read(socket_, asio::buffer(msg, buf_size), asio::transfer_exactly(bytes), ec);
  if(ec)
  {
    printf("error occurred in read_exactly");
    return init_failed;
  }
  return ok;
}

return_statuses TCPInterface::read_some(unsigned char *msg, size_t buf_size, size_t &bytes_read)
{
  if (!ok_)
    return init_failed;

  boost::system::error_code ec;
  bytes_read = socket_.read_some(asio::buffer(msg, buf_size), ec);
  if(ec)
  {
    printf("error occurred in read_some");
    return init_failed;
  }
  return ok;
}

return_statuses TCPInterface::send(unsigned char *msg, size_t size)
{
  if (!ok_)
    return init_failed;

  socket_.send(asio::buffer(msg, size));

  return ok;
}
