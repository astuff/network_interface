FROM ros:kinetic-ros-base

# Set environmental variable for repo names
ENV reponame network_interface
ENV workspace /root/driver_ws

# Install prereqs for package
RUN apt-get update && apt-get install -y \
    python-catkin-tools \
    linux-headers-generic-hwe-16.04 \
    && rm -rf /var/lib/apt/lists/

# Create workspace for installing and building packages
RUN mkdir -p ${workspace}/src/${reponame}

# Copy this repo's contents into the workspace folder
ADD . ${workspace}/src/${reponame}

# Set up and build the workspace
RUN cd ${workspace} \
    && catkin init \
    && catkin config --extend /opt/ros/kinetic \
    && catkin build
