^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package network_interface
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Forthcoming
-----------
* Updating package.xml to format 2.
* Re-releasing under MIT license.
* Removing unused header.
* Fixing type-punned pointer isssues.
* Adding utility header.
* Cleaning up function formatting and some const refs.
* Adding README.
* Removing roscpp from list of dependencies.
* Updating repo URLs.
* Adding Travis CI integration. Bumping version.
* Adding is_open functions for tcp and udp.
* Fixing license typos.
* Standardizing interface error handling.
  Added additional error values BAD_PARAM and SOCKET_CLOSED.
  Removed ni_error_handler in favor of return_status_desc.
* First pass at standardizing reads, writes, and error reporting in network_interface.
* read_some on TCP was not returning the number of bytes read.
* Changing license to GPLv3.
* Changing message name to ROS standard format.
* Fixing catkin_package line to include the correct directory.
* added read_exactly message to tcp. adds size to TCPFrame
* added tcp_interface. renamed packaged to network_interface
* renamed package, added tcp interface, renamed header and namespace
* Initial version
* Contributors: Daniel Stanek, Joe Kale, Joshua Whitley
