cmake_minimum_required (VERSION 3.8)

project (c-torrent)



include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main # Important: Specify a version or a commit hash here.
)
FetchContent_MakeAvailable(ftxui)
 
project(c-torrent LANGUAGES CXX VERSION 1.0.0)
add_executable(c-torrent 
	main.cpp 
	bencode/bencoder.cpp 
	bencode/file_parser.cpp)
target_link_libraries(c-torrent
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
)