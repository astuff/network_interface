cmake_minimum_required(VERSION 2.8.3)
project(network_interface)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
    roscpp
    std_msgs
    message_generation)

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  udp_frame.msg
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs  # Or other packages containing msgs
)

catkin_package(
    INCLUDE_DIRS include/${PROJECT_NAME}
    CATKIN_DEPENDS std_msgs message_runtime
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(include/${PROJECT_NAME}
    ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
add_library(network_interface
  src/udp_interface.cpp
  src/tcp_interface.cpp
)

## Specify libraries to link a library or executable target against
target_link_libraries(network_interface
    ${catkin_LIBRARIES}
)

#############
## Install ##
#############

## Mark executables and/or libraries for installation
install(TARGETS network_interface
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)
