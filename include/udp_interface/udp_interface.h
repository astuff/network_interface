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

// Define a class that supports a basic CAN interface that's independent of the hardware and driver library used
// Different libraries can be created to define all these functions for a specific driver library

#ifndef UDP_INTERFACE_HPP
#define UDP_INTERFACE_HPP

//C++ Includes
#include <iostream>

//OS Includes
#include <unistd.h>
#include <netinet/in.h>

namespace AS
{
namespace UDP
{
enum return_statuses
{
  ok = 0,
  init_failed,
  no_messages_received,
  send_failed
};

class UDPInterface
{
public:
  UDPInterface();

  ~UDPInterface();

  // Called to pass in parameters and open ethernet link
  return_statuses open(const char *local_address,
                       const char *remote_address,
                       int local_port,
                       int remote_port);

  // Close the ethernet link
  return_statuses close();

  // Read a message
  return_statuses read(unsigned char *msg, unsigned int *size, unsigned int buf_size);

  // Send a message
  return_statuses send(unsigned char *msg, unsigned int size);

private:
  int socket_;
  in_addr_t remote_addr_;
  int remote_port_;
  bool ok_;
};

}
}
#endif
