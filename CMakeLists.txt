cmake_minimum_required(VERSION 3.16)
project(line_tracer)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(include)
include_directories(/usr/local/include/dynamixel_sdk)
#build
add_executable(jpub src/jetsonpub.cpp src/jetson_pub_main.cpp src/dxl.cpp /home/jetson/ros2_ws/src/ltp_jetson/src/8_lt_cw_100rpm_in.mp4)
ament_target_dependencies(jpub rclcpp sensor_msgs cv_bridge OpenCV)
target_link_libraries(jpub dxl_x64_cpp)
add_executable(jsub src/jetsonsub.cpp src/jetson_sub_main.cpp src/dxl.cpp)
ament_target_dependencies(jsub rclcpp sensor_msgs cv_bridge OpenCV)
target_link_libraries(jsub dxl_x64_cpp)
add_executable(vm src/vm.cpp src/vm_main.cpp src/dxl.cpp)
ament_target_dependencies(vm rclcpp sensor_msgs std_msgs OpenCV)
target_link_libraries(vm dxl_x64_cpp)

# Install
install(TARGETS
  jpub
  jsub
  vm
  DESTINATION lib/${PROJECT_NAME})

ament_package()
