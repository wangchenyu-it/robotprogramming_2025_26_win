cmake_minimum_required (VERSION 2.8.11)
project (rp_sim)

set(CMAKE_BUILD_TYPE Release)

find_package(OpenCV REQUIRED)
add_executable(rp_sim
        grid.cpp
        grid_map.cpp
        world.cpp
        canvas.cpp
        differential_drive_robot.cpp
        lidar.cpp
        rp_sim.cpp)

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

target_link_libraries(rp_sim
  ${OpenCV_LIBRARIES}
  m
  )


