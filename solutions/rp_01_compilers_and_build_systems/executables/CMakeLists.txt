# TODO: compile executable that will be run
add_executable(many_objects_in_stack_example
    many_object_in_stack_example.cpp)

# TODO: link the executable against the core_library
target_link_libraries(many_objects_in_stack_example core_library)
