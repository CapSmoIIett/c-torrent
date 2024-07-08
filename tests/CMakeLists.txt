cmake_minimum_required (VERSION 3.8)

project(gtests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable (tests 
	bencode_test.cpp
	file_parser_test.cpp
	sha1_test.cpp
	request_test.cpp
	
	../c-torrent/bencoder.cpp
	../c-torrent/file_parser.cpp
	../c-torrent/sha1.cpp
	../c-torrent/socket.cpp
	../c-torrent/peer.cpp
	../c-torrent/torrent.cpp)

target_link_libraries(
  tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

#target_include_directories( tests PRIVATE ${GTEST_INCLUDE_DIRS} )
#target_link_libraries( tests ${GTEST_BOTH_LIBRARIES} )