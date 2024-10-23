CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(simple_simulator)


# Set the output directory for the build executables and libraries
ADD_DEFINITIONS(-DUNIX)
MESSAGE(STATUS "Compiling on Unix")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG") 
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG") 
SET(CMAKE_BUILD_TYPE Release)

# Find Eigen3
# FIND_PACKAGE(Eigen3 REQUIRED)
# INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})
INCLUDE_DIRECTORIES(/usr/include/eigen3)

# Find Opencv
FIND_PACKAGE(OpenCV  REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})


add_executable(simple_simulator_test
  src/simple_simulator_test.cpp 
  src/world.cpp
  src/robot.cpp
  src/lidar.cpp
  )

target_link_libraries(
  simple_simulator_test 
  ${OpenCV_LIBS} 
)



