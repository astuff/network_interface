cmake_minimum_required(VERSION 2.8.3)
project(network_interface)

add_definitions(-std=c++11 -Wall -Wextra)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

## Find catkin macros and libraries
find_package(
  catkin REQUIRED
  COMPONENTS
  message_generation
  roslint
  std_msgs
)

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  TCPFrame.msg
  UDPFrame.msg
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs  # Or other packages containing msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS message_runtime std_msgs
)

###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_library(
  ${PROJECT_NAME}
  src/tcp_interface.cpp
  src/udp_interface.cpp
  src/udp_server.cpp
  src/utils.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  ${catkin_LIBRARIES}
)

##########
## TEST ##
##########

roslint_cpp()

if(CATKIN_ENABLE_TESTING)
  roslint_add_test()
  catkin_add_gtest(${PROJECT_NAME}_tests test/network_interface_test.cpp)
  target_link_libraries(${PROJECT_NAME}_tests ${catkin_LIBRARIES})
endif()

#############
## Install ##
#############

## Mark executables and/or libraries for installation
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)
