cmake_minimum_required(VERSION 3.5)
project(network_interface)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()


## Generate messages in the 'msg' folder
rosidl_generate_interfaces(${PROJECT_NAME}_msgs
  "msg/TCPFrame.msg"
  "msg/UDPFrame.msg"
  DEPENDENCIES builtin_interfaces std_msgs
)


###########
## Build ##
###########

include_directories(
  include
)

add_library(
  ${PROJECT_NAME}
  src/tcp_interface.cpp
  src/udp_interface.cpp
  src/udp_server.cpp
  src/utils.cpp
)

ament_export_targets(${PROJECT_NAME} HAS_LIBRARY_TARGET)

#############
## Install ##
#############

install(
	DIRECTORY include/network_interface
	DESTINATION include
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

ament_package()