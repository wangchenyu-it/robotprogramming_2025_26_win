add_library(core_library SHARED
    utils/property_eigen.cpp
    utils/blob.cpp
    utils/id_context.cpp
    utils/serializer.cpp
    utils/property.cpp
    utils/property_identifiable.cpp
    utils/deserializer.cpp
    utils/property_container.cpp
    utils/property_container_manager.cpp
    utils/serializable.cpp
    utils/serialization_context.cpp
    utils/id_placeholder.cpp
    utils/identifiable.cpp
    utils/object_data.cpp
    messages/pose_message.cpp
    messages/base_sensor_message.cpp
    messages/odometry_message.cpp

    # TODO add other .cpp files
)

# TODO: Include the current directory ( . ) to the list of directories to search for header files
target_include_directories(core_library PUBLIC .)

# TODO: Include the Eigen3 library to the list of libraries to link against
target_link_libraries(core_library PUBLIC Eigen3::Eigen)
