cmake_minimum_required(VERSION 3.14)
project(simple-chess-games VERSION 0.0.1 DESCRIPTION "Simple library to play chess games")
add_library(simple-chess-games SHARED
	src/Board.cpp
	src/Color.cpp
	src/Exceptions.cpp
	src/Game.cpp
	src/GameStage.cpp
	src/Piece.cpp
	src/PieceMove.cpp
	src/PlayedMove.cpp
	src/Square.cpp
	src/details/BoardAnalyzer.cpp
	src/details/DrawEvaluator.cpp
	src/details/MoveValidator.cpp
	src/details/fen/FenParser.cpp
	src/details/fen/FenUtils.cpp
	src/details/moves/BishopMove.cpp
	src/details/moves/KingMove.cpp
	src/details/moves/KnightMove.cpp
	src/details/moves/PawnMove.cpp
	src/details/moves/QueenMove.cpp
	src/details/moves/RookMove.cpp
	)

set(CMAKE_CXX_STANDARD 11)
set(COMMON_FLAGS -Wall -Wextra -Wpedantic -Werror)

# Enable debug
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_target_properties(simple-chess-games PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER src/Board.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER src/Color.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER src/Exceptions.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER src/Game.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER src/GameStage.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER src/Piece.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER src/PieceMove.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER src/PlayedMove.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER src/Square.h)
target_include_directories(simple-chess-games PRIVATE src)

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

install(TARGETS simple-chess-games
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

enable_testing()
add_executable(run_tests
	tests/AlgebraicNotation_test.cpp
	tests/DrawDetection_test.cpp
	tests/FenGeneration_test.cpp
	tests/GameCreation_test.cpp
	tests/MoveAvailability_test.cpp
	tests/MoveCounter_test.cpp
	tests/MovesOnBoard_test.cpp
	tests/Resignation_test.cpp)
target_link_libraries(run_tests gtest_main simple-chess-games)

include(GoogleTest)
include_directories(src)
gtest_discover_tests(run_tests)
