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

#include <cstring>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

using namespace std;
using namespace AS::Network;
using AS::Network::return_statuses;

//Default constructor.
UDPInterface::UDPInterface() :
        ok_(false)
{
}

//Default destructor.
UDPInterface::~UDPInterface()
{
}

return_statuses UDPInterface::open(const char *local_address,
                                   const char *remote_address,
                                   int local_port,
                                   int remote_port)
{
    // Initialize socket
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ < 0)
        return init_failed;

    // Allow re-use
    int reuse = 1;
    int err = setsockopt(socket_,
                         SOL_SOCKET,
                         SO_REUSEADDR,
                         (char *) &reuse,
                         sizeof(reuse));
    if (err < 0)
        return init_failed;

    // Prevent blocking
    int nonBlocking = 1;
    err = fcntl(socket_, F_SETFL, O_NONBLOCK, nonBlocking);

    if (err < 0)
        return init_failed;

    in_addr_t local_addr = inet_addr(local_address);
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons((unsigned short) local_port);
    sockaddr.sin_addr.s_addr = local_addr;

    err = bind(socket_, (struct sockaddr*) &sockaddr, sizeof(sockaddr));

    if (err < 0)
        return init_failed;

    remote_addr_ = inet_addr(remote_address);
    remote_port_ = remote_port;

    ok_ = true;

    return ok;
}

return_statuses UDPInterface::close()
{
    if (!ok_)
        return init_failed;

    shutdown(socket_, SHUT_RDWR);

    return ok;
}

return_statuses UDPInterface::read(unsigned char *msg, unsigned int *size, unsigned int buf_size)
{
    if (!ok_)
        return init_failed;

    struct sockaddr from_socket_addr;
    socklen_t from_socket_addr_len;
    ssize_t rx_length;
    return_statuses return_val = no_messages_received;

    bool done = false;
    while (!done)
    {
        from_socket_addr_len = sizeof(from_socket_addr);
        rx_length = recvfrom(socket_,
                             (void *) msg,
                             buf_size,
                             0,
                             &from_socket_addr,
                             &from_socket_addr_len);

        if (rx_length <= 0)
        {
            *size = 0;
            done = true;
        }
        else
        {
            if ((from_socket_addr_len == sizeof(sockaddr_in)) &&
                (from_socket_addr.sa_family == AF_INET))
            {
                struct sockaddr_in *addr_in = (struct sockaddr_in *) &from_socket_addr;

                if (((remote_addr_ == INADDR_ANY) || (remote_addr_ == addr_in->sin_addr.s_addr)) &&
                    ((remote_port_ == 0) || (remote_port_ == addr_in->sin_port)))
                {
                    *size = (unsigned int) rx_length;
                    done = true;
                    return_val = ok;
                }
            }
        }
    }

    return return_val;
}

return_statuses UDPInterface::send(unsigned char *msg, unsigned int size)
{
    if (!ok_)
        return init_failed;

    struct sockaddr_in to_socket_addr;
    memset(&to_socket_addr, 0, sizeof(sockaddr));
    to_socket_addr.sin_family = AF_INET;
    to_socket_addr.sin_port = htons((unsigned short) remote_port_);
    to_socket_addr.sin_addr.s_addr = remote_addr_;

    ssize_t tx_length;
    tx_length = sendto(socket_,
                       (const void *) msg,
                       size,
                       0,
                       (struct sockaddr *) &to_socket_addr,
                       sizeof(to_socket_addr));

    if (tx_length != size)
        return send_failed;

    return ok;
}
