cmake_minimum_required (VERSION 3.8)
project (dmap_localizer LANGUAGES C CXX CUDA)

set(CMAKE_BUILD_TYPE Release)

find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --expt-relaxed-constexpr -diag-suppress=20011 -diag-suppress=20014")
set(CMAKE_CUDA_ARCHITECTURES native)


include_directories(${EIGEN3_INCLUDE_DIR}
${OpenCV_INCLUDE_DIRS}
${CUDA_INCLUDE_DIRS}
)

include_directories(${OpenCV_INCLUDE_DIRS})

add_library(dmap_localizer_library
            SHARED
            grid.cu
            dmap.cpp
            dmap_localizer.cpp)

target_link_libraries(dmap_localizer_library
        ${OpenCV_LIBRARIES}
)

set_target_properties(dmap_localizer_library PROPERTIES
                      CUDA_SEPARABLE_COMPILATION ON)
                      
add_executable(dmap_localizer_test
               dmap_localizer_test.cpp)

target_link_libraries(dmap_localizer_test
                      dmap_localizer_library)

add_executable(simple_cuda
               simple_cuda.cu)

