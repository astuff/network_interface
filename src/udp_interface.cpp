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

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

using namespace std;
using namespace AS::Network;

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
