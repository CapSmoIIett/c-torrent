cmake_minimum_required (VERSION 3.8)

project(gtests)


find_package(OpenSSL COMPONENTS Crypto SSL QUIET)


if (TARGET OpenSSL::SSL AND TARGET OpenSSL::Crypto)
	set(USING_OPENSSL ${OPENSSL_FOUND})
	if (${OPENSSL_FOUND})
		message("Use open ssl")
		add_compile_definitions(CPPHTTPLIB_OPENSSL_SUPPORT)
	else()
		message("Don't use open ssl")
	endif()
else()
	message("Don't use open ssl")
	set(USING_OPENSSL FALSE)
endif()

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
	writer_test.cpp
	
	../c-torrent/bencoder.cpp
	../c-torrent/file_parser.cpp
	../c-torrent/sha1.cpp
	../c-torrent/socket.cpp
	../c-torrent/peer.cpp
	../c-torrent/torrent.cpp
	../c-torrent/logger.cpp
	../c-torrent/async_writer.cpp)

target_link_libraries(
  tests
  GTest::gtest_main
  #OpenSSL::SSL
  #OpenSSL::Crypto
)

if (USING_OPENSSL)
target_link_libraries(
  tests
  OpenSSL::SSL
  OpenSSL::Crypto
)
endif()

include(GoogleTest)
gtest_discover_tests(tests)

#target_include_directories( tests PRIVATE ${GTEST_INCLUDE_DIRS} )
#target_link_libraries( tests ${GTEST_BOTH_LIBRARIES} )