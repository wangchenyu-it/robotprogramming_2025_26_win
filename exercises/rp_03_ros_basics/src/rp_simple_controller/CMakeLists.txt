cmake_minimum_required(VERSION 3.8)
project(rp_simple_controller)

# Hopefully these should make the code more portable
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

# Find the other dependencies [geometry_msgs and turtlesim]
# TODO here!

# Adding new node turtle_square
add_executable(turtle_square src/turtle_square.cpp)

# Link the ament dependencies to turtle_square using the
# ament_target_dependencies function.
# Remember to link the dependencies, you should write:
# ament_target_dependencies(target_name dependency1 dependency2 ...)
# TODO here!

# Install the executable to the lib/${PROJECT_NAME} directory
install(
	TARGETS
	turtle_square
	DESTINATION
	lib/${PROJECT_NAME}
)

# Finaly, call ament_package to make the package.
# This should be the last line in the CMakeLists.txt file.
ament_package()
