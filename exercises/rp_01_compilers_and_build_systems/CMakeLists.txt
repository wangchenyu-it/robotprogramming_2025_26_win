cmake_minimum_required(VERSION 2.8.3)
project(mini_core)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

# TODO invokes the routines to find a package called Eigen3

# add project source code
add_subdirectory(src)

# TODO do the same for executables
