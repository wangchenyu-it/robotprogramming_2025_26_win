cmake_minimum_required(VERSION 2.8.3)
project(lesson_5)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

add_executable(sort_1 sort_1.cpp)
add_executable(sort_2 sort_2.cpp)
add_executable(sort_3 sort_3.cpp)
add_executable(list_example  list_example.cpp)
add_executable(vec_read_example vec_read_example.cpp)

add_executable(stl_list stl_list.cpp)
add_executable(stl_vector stl_vector.cpp)
add_executable(stl_map stl_map.cpp)
