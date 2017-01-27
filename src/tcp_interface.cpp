/*
* AutonomouStuff, LLC ("COMPANY") CONFIDENTIAL
* Unpublished Copyright (c) 2009-2017 AutonomouStuff, LLC, All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains the property of COMPANY. The intellectual and technical concepts contained
* herein are proprietary to COMPANY and may be covered by U.S. and Foreign Patents, patents in process, and are protected by trade secret or copyright law.
* Dissemination of this information or reproduction of this material is strictly forbidden unless prior written permission is obtained
* from COMPANY.  Access to the source code contained herein is hereby forbidden to anyone except current COMPANY employees, managers or contractors who have executed
* Confidentiality and Non-disclosure agreements explicitly covering such access.
*
* The copyright notice above does not evidence any actual or intended publication or disclosure  of  this source code, which includes
* information that is confidential and/or proprietary, and is a trade secret, of  COMPANY.   ANY REPRODUCTION, MODIFICATION, DISTRIBUTION, PUBLIC  PERFORMANCE,
* OR PUBLIC DISPLAY OF OR THROUGH USE  OF THIS  SOURCE CODE  WITHOUT  THE EXPRESS WRITTEN CONSENT OF COMPANY IS STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE
* LAWS AND INTERNATIONAL TREATIES.  THE RECEIPT OR POSSESSION OF  THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR IMPLY ANY RIGHTS
* TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL ANYTHING THAT IT  MAY DESCRIBE, IN WHOLE OR IN PART.
*/

#include <network_interface.h>

#include <cstring>

using namespace AS::Network;
using namespace std;
using namespace boost;
using boost::asio::ip::tcp;

//Default constructor.
TCPInterface::TCPInterface() :
  ok_(false), socket_(io_service_)
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

return_statuses TCPInterface::read(unsigned char *msg, size_t buf_size)
{
  if (!ok_)
    return init_failed;

  return_statuses return_val = no_messages_received;
  bool done = false;
  while (!done)
  {
    int rcvSize = socket_.read_some(asio::buffer(msg, buf_size));
    if(rcvSize <= 0)
    {
      done = true;
    }
    else
    {
      done = true;
      return_val = ok;
    }
  }

  return return_val;
}

return_statuses TCPInterface::send(unsigned char *msg, size_t size)
{
  if (!ok_)
    return init_failed;

  socket_.send(asio::buffer(msg, size));

  return ok;
}
