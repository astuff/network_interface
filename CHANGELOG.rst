^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package network_interface
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

3.1.0 (2021-05-21)
------------------
* Fix CMakeLists.txt include install (`#40 <https://github.com/astuff/network_interface/issues/40>`_)
* UDP Server: Don't send reply if zero length (`#38 <https://github.com/astuff/network_interface/issues/38>`_)
* Add stop function for terminating (`#36 <https://github.com/astuff/network_interface/issues/36>`_)
* UDP Server Class (`#35 <https://github.com/astuff/network_interface/issues/35>`_)
* Fix UDP read function, use buffer of fixed size and shrink down after receiving packet (`#33 <https://github.com/astuff/network_interface/issues/33>`_)
* Contributors: icolwell-as

3.0.0 (2021-03-03)
------------------
* Add noetic CI builds (`#31 <https://github.com/astuff/network_interface/issues/31>`_)
* Disable docker layer caching (`#30 <https://github.com/astuff/network_interface/issues/30>`_)
* Merge pull request `#26 <https://github.com/astuff/network_interface/issues/26>`_ from astuff/fix/memory_allocation
  Fix memory initialization of buffer vector.
* Apply same buffer allocation fix to UDP interface.
* Fix memory initialization of buffer vector.
* Adding copyright to utils.cpp.
* Moving definition of return_status_desc to fix linking bug.
* Merge pull request `#25 <https://github.com/astuff/network_interface/issues/25>`_ from astuff/maint/cpp_11_cleanup
  Maint/cpp 11 cleanup
* Had to add offset to parse_tuple.
* Moving parse_tuple from ibeo_core.
* Allowing read functions to take multiple containers.
* Commenting code.
* New implementation of write_le and write_be. Tests for both.
* Changing valueOffset to float and casting in util functions.
* Replacing unsigned int with uint32_t.
* Used wrong type-trait qualifier.
* CI: Adding Melodic armhf build.
* CI: Machine executors require sudo.
* CI: Add first run at armhf build.
* Changing to reinterpret_cast instead of non-portable solution.
* Removing async calls from TCP. Allocating buffers with reserve.
* Removing apparently useless check in write_le.
* Consistent braced initializers.
* -DBUILD_ROS is no longer a thing.
* Reserving requested space in read_exactly.
* Making ByteOrder enum scoped too.
* Switching back to memcpy.
* Moving common enums into common.h.
* Scoped enum for return statuses.
* Using vectors and type traits.
* CI: Making roslint run in run_tests.
* Merge pull request `#20 <https://github.com/astuff/network_interface/issues/20>`_ from astuff/maint/ci_add_lint
  Maint/ci add lint
* CI: Adding catkin_test_results the right way.
* CI: Adding catkin_test_results.
* Making lint cover all files and adding copyright.
* CI: Need --no-deps for lint.
* CI: Oops.
* CI: Adding roslint to build tests.
* Removing unused gitlabci file.
* CI: Removing Indigo build.
* Merge pull request `#19 <https://github.com/astuff/network_interface/issues/19>`_ from astuff/maint/add_urls
  Adding website URL to package.xml.
* Adding website URL to package.xml.
* Merge pull request `#18 <https://github.com/astuff/network_interface/issues/18>`_ from astuff/maint/ci_new_docker_images
  CI: Using AS Docker images.
* CI: Using AS Docker images.
* Contributors: Ian Colwell, Joshua Whitley, Rinda Gunjala, Sam Rustan, Zach Oakes

2.1.0 (2018-08-30)
------------------
* Merge pull request `#13 <https://github.com/astuff/network_interface/issues/13>`_ from astuff/maint/remove_unused_variables_in_test
* Minor linting stuff.
* Implementing proper dependencies for rosunit.
* Removing unused variables in gtests.
* Merge pull request `#10 <https://github.com/astuff/network_interface/issues/10>`_ from astuff/maint/roslint
* Removing temporary .orig files.
* Fixing roslint suggestions.
* Adding roslint.
* Merge pull request `#8 <https://github.com/astuff/network_interface/issues/8>`_ from astuff/fix/strict_aliasing
* endianess check now returns bool
* removes copying input into system endianess check
* Adds check for system endianess
  This allows moving the source address start further into
  the stored uint64_t to properly grab the least significant
  bytes for putting into smaller containers.
* changes comment on readbeint test case
* removes unused include from debugging
* Fixes read_be and read_le to remove aliasing issue
  Also adds .vscode to gitignore for VS Code users.
* Merge pull request `#6 <https://github.com/astuff/network_interface/issues/6>`_ from astuff/unit_testing
* Adds unit tests for some methods. Corrects found issues
  Unit tests found issues with misinterpreting floats and doubles
  Modified methods and now this commit Fixes `#1 <https://github.com/astuff/network_interface/issues/1>`_
* Merge pull request `#3 <https://github.com/astuff/network_interface/issues/3>`_ from KyleARectorAStuff/feature/tcp_read_timeout
* Adding no-timeout option when the timeout_ms argument is 0
  Before this commit, a timeout was used in every call TCPInterface::read or
  TCPInterface::read_exactly, with a default of 0 ms. After this commit,
  the default is set to 0 ms, and if the read or read_exactly methods receive
  a 0 timeout request, it will not set a deadline for the timeout, resulting
  in a blocking read. This allows for the TCPInterface to behave with a
  timeout, or else be used as it was previously.
* Removing timeout/received flags, adding error checking in timeout handler
  Before this commit, the result of a read or timeout was stored in a private variable,
  populated by the respective callback. After this commit, the conditional statements that
  proviously relied on the flags instead rely on the gloabl error message's value.
  Additionally, the timeout handler has added error checking to prevent it from executing fully
  when the timer.cancel() method is called after a successful read.
* Adding TCP timout to TCPInterface::read_exactly function
  Before this commit, the read_exactly method used a blocking read call. After this commit, the
  read_exactly function has a configurable timeout in milliseconds, with a default of 5 ms.
* Parameterizing timeout value and setting default to 5 ms
  Before this commit, the timeout for the TCPInterface::read() method had a hard-coded timeout
  value of 5 ms. After this commit, the TCPInterface::read() function takes an optional
  parameter for the timeout, in milliseconds. This parameter defaults to 5 ms.
* Removing while loop with io_service\_.run_one() condition for correct execution
  Before this commit, the tcp_interface read method would constantly return a timeout error, even
  if data had been read properly. After this commit, the read method returns an OK status if the read
  was successful, or TIMEOUT or READ_FAILED depending on the failure type. In the Boost asio library,
  the io_service can be run continuously, or run once until an event hander has been dispatched. The
  return value of the run_one method was previously used as a while loop exit condition, but this
  resulted in the initial behavior describe above, as if the run_one method actually returned after
  several even handlers were dispatched, instead of just one. After removing the while loop and using
  the method alone, the desired behavior was achieved.
* Initial implementation of timeout on TCP read
* Contributors: Daniel-Stanek, Joe Kale, Joshua Whitley, Kyle Rector, Lucas Buckland, Nishanth Samala, Sam Rustan, Samuel Rustan, Zach Oakes

2.0.0 (2018-04-25)
------------------
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
