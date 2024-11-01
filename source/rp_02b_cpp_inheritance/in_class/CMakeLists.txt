cmake_minimum_required (VERSION 2.8.11)
project (rp_sim)

find_package(OpenCV REQUIRED)

add_executable(rp_sim
        grid.cpp
        grid_map.cpp
        rp_sim.cpp
        world.cpp)

include_directories(${OPENCV_INCLUDE_DIRS})

target_link_libraries(rp_sim
  ${OpenCV_LIBRARIES}
  m
  )


