cmake_minimum_required(VERSION 3.14)
project(simple-chess-games VERSION 0.0.1 DESCRIPTION "Simple library to play chess games")

# Core chess engine sources
set(core_sources
	src/core/Board.cpp
	src/core/Builders.cpp
	src/core/Color.cpp
	src/core/Exceptions.cpp
	src/core/Game.cpp
	src/core/GameManagerImpl.cpp
	src/core/GameStage.cpp
	src/core/Piece.cpp
	src/core/PieceMove.cpp
	src/core/PlayedMove.cpp
	src/core/Square.cpp
	src/core/details/AlgebraicNotationGenerator.cpp
	src/core/details/BoardAnalyzer.cpp
	src/core/details/DrawEvaluator.cpp
	src/core/details/GameStageUpdater.cpp
	src/core/details/GameStateDetector.cpp
	src/core/details/MoveValidator.cpp
	src/core/details/fen/FenParser.cpp
	src/core/details/fen/FenUtils.cpp
	src/core/details/moves/BishopMove.cpp
	src/core/details/moves/KingMove.cpp
	src/core/details/moves/KnightMove.cpp
	src/core/details/moves/PawnMove.cpp
	src/core/details/moves/QueenMove.cpp
	src/core/details/moves/RookMove.cpp)

# C++ interface sources
set(cpp_interface_sources
	src/cpp_interface/GameManager.cpp)

# C interface sources
set(c_interface_sources
	src/c_interface/chess_c_impl.cpp
	src/c_interface/conversion_utils.cpp)

# All C++ sources (core + C++ interface)
set(cpp_library_sources ${core_sources} ${cpp_interface_sources})

# All sources (core + both interfaces)
set(all_sources ${core_sources} ${cpp_interface_sources} ${c_interface_sources})

set(CMAKE_CXX_STANDARD 17)
set(COMMON_FLAGS -Wall -Wextra -Wpedantic -Werror)

# Enable debug
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ===== C++ LIBRARY =====

# Shared C++ library
add_library(simple-chess-games SHARED ${cpp_library_sources})
target_include_directories(simple-chess-games PUBLIC include)
target_link_libraries(simple-chess-games PRIVATE Boost::algorithm Boost::bimap)

# Static C++ library
add_library(simple-chess-games-static STATIC ${cpp_library_sources})
target_include_directories(simple-chess-games-static PUBLIC include)
target_link_libraries(simple-chess-games-static PRIVATE Boost::algorithm Boost::bimap)

# Set properties for C++ libraries
set_target_properties(simple-chess-games PROPERTIES
	VERSION ${PROJECT_VERSION}
	PUBLIC_HEADER "include/cpp/simplechess/Board.h;include/cpp/simplechess/Color.h;include/cpp/simplechess/Exceptions.h;include/cpp/simplechess/Game.h;include/cpp/simplechess/GameManager.h;include/cpp/simplechess/GameStage.h;include/cpp/simplechess/Piece.h;include/cpp/simplechess/PieceMove.h;include/cpp/simplechess/PlayedMove.h;include/cpp/simplechess/Square.h")

set_target_properties(simple-chess-games-static PROPERTIES
	VERSION ${PROJECT_VERSION}
	PUBLIC_HEADER "include/cpp/simplechess/Board.h;include/cpp/simplechess/Color.h;include/cpp/simplechess/Exceptions.h;include/cpp/simplechess/Game.h;include/cpp/simplechess/GameManager.h;include/cpp/simplechess/GameStage.h;include/cpp/simplechess/Piece.h;include/cpp/simplechess/PieceMove.h;include/cpp/simplechess/PlayedMove.h;include/cpp/simplechess/Square.h")

# ===== C LIBRARY =====

# Shared C library (includes both core and C interface)
add_library(simple-chess-games-c SHARED ${all_sources})
target_include_directories(simple-chess-games-c PUBLIC include)
target_link_libraries(simple-chess-games-c PRIVATE Boost::algorithm Boost::bimap)

# Static C library
add_library(simple-chess-games-c-static STATIC ${all_sources})
target_include_directories(simple-chess-games-c-static PUBLIC include)
target_link_libraries(simple-chess-games-c-static PRIVATE Boost::algorithm Boost::bimap)

# Set properties for C libraries
set_target_properties(simple-chess-games-c PROPERTIES
	VERSION ${PROJECT_VERSION}
	PUBLIC_HEADER "include/c/simplechess/chess.h")

set_target_properties(simple-chess-games-c-static PROPERTIES
	VERSION ${PROJECT_VERSION}
	PUBLIC_HEADER "include/c/simplechess/chess.h")

include(GNUInstallDirs)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Boost
FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.82.0/boost-1.82.0.tar.xz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(BOOST_ENABLE_CMAKE ON)
# Only build the libraries we need
set(BOOST_INCLUDE_LIBRARIES algorithm bimap optional)
FetchContent_MakeAvailable(Boost)

# Install targets
install(TARGETS simple-chess-games simple-chess-games-c
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS simple-chess-games-static simple-chess-games-c-static
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install directory structure for headers
install(DIRECTORY include/cpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/c DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Tests
enable_testing()

# C++ Tests
add_executable(run_cpp_tests
	tests/cpp/AlgebraicNotation_test.cpp
	tests/cpp/DrawDetection_test.cpp
	tests/cpp/FenGeneration_test.cpp
	tests/cpp/GameCreation_test.cpp
	tests/cpp/MoveAvailability_test.cpp
	tests/cpp/MoveCounter_test.cpp
	tests/cpp/MovesOnBoard_test.cpp
	tests/cpp/Resignation_test.cpp)
target_include_directories(run_cpp_tests PRIVATE include)
target_include_directories(run_cpp_tests PRIVATE src)
target_include_directories(run_cpp_tests PRIVATE tests/cpp)
target_link_libraries(run_cpp_tests gtest_main simple-chess-games Boost::optional)

# C Interface Tests
add_executable(run_c_tests
	tests/c/AlgebraicNotation_test.cpp)
target_include_directories(run_c_tests PRIVATE include)
target_include_directories(run_c_tests PRIVATE src)
target_link_libraries(run_c_tests gtest_main simple-chess-games-c)

include(GoogleTest)
gtest_discover_tests(run_cpp_tests)
gtest_discover_tests(run_c_tests)

# Backward compatibility - run_tests now runs C++ tests
add_custom_target(run_tests DEPENDS run_cpp_tests)