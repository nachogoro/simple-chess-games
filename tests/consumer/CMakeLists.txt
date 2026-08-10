# A consumer of the installed library, built out of tree against nothing but
# the install prefix. It exists so that the install rules and the exported
# package are exercised: the main build can pass while the thing people
# actually download is unusable.

cmake_minimum_required(VERSION 3.14)
project(simple-chess-games-consumer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(simple-chess-games REQUIRED)

add_executable(consumer_cpp main.cpp)
target_link_libraries(consumer_cpp PRIVATE SimpleChess::simple-chess-games)

add_executable(consumer_c main.c)
target_link_libraries(consumer_c PRIVATE SimpleChess::simple-chess-games-c)

# The static libraries have to be usable on their own terms too.
add_executable(consumer_cpp_static main.cpp)
target_link_libraries(consumer_cpp_static
	PRIVATE SimpleChess::simple-chess-games-static)

add_executable(consumer_c_static main.c)
target_link_libraries(consumer_c_static
	PRIVATE SimpleChess::simple-chess-games-c-static)
