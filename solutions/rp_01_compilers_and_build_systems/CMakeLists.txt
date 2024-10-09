cmake_minimum_required(VERSION 2.8.3)
project(mini_core)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused")

# TODO invokes the routines to find a package called Eigen3
find_package(Eigen3 REQUIRED)

# add project source code
add_subdirectory(src)

# TODO do the same for executables
add_subdirectory(executables)
