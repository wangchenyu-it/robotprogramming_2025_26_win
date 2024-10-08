cmake_minimum_required(VERSION 2.8.3)
project(mini_core)

set(CMAKE_CXX_STANDARD 11)

# TODO invokes the routines to find a package called Eigen3
find_package(Eigen3 REQUIRED)

# add project source code
add_subdirectory(src)

# TODO do the same for executables
add_subdirectory(executables)
