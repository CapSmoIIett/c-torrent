cmake_minimum_required (VERSION 3.8)

find_package(GTest REQUIRED)

add_executable (tests )

target_include_directories( tests PRIVATE ${GTEST_INCLUDE_DIRS} )
target_link_libraries( tests ${GTEST_BOTH_LIBRARIES} )