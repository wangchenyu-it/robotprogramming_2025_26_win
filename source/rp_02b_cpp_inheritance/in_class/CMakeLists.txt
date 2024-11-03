cmake_minimum_required (VERSION 2.8.11)
project (rp_sim)

set(CMAKE_BUILD_TYPE Release)

find_package(OpenCV REQUIRED)
add_executable(rp_sim
        grid.cpp
        grid_map.cpp
        rp_sim.cpp
        world.cpp
        canvas.cpp)

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

target_link_libraries(rp_sim
  ${OpenCV_LIBRARIES}
  m
  )


