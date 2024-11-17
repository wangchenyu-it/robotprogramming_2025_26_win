cmake_minimum_required (VERSION 3.5)
project (dmap_localizer)

set(CMAKE_BUILD_TYPE Release)

find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)


include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(dmap_localizer_library
            dmap.cpp
            dmap_localizer.cpp)

target_link_libraries(dmap_localizer_library ${OpenCV_LIBRARIES})

add_executable(dmap_localizer_test
               dmap_localizer_test.cpp)

target_link_libraries(dmap_localizer_test
                      dmap_localizer_library)

