add_library(btree_library btree.cpp)
add_executable(btree_test btree_test.cpp)
target_link_libraries(btree_test
  btree_library)
