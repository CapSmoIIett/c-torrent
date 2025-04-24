# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (c-torrent VERSION 0.1 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)

include(CTest)

add_subdirectory (c-torrent)
add_subdirectory (tests)