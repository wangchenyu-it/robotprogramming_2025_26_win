add_library(core_library SHARED
    utils/property_eigen.cpp
    utils/blob.cpp
    utils/id_context.cpp
    utils/serializer.cpp

    # TODO add other .cpp files
)

# TODO: Include the current directory ( . ) to the list of directories to search for header files

# TODO: Include the Eigen3 library to the list of libraries to link against
