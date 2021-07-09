cmake_minimum_required(VERSION 3.9)
project(simple-chess-games VERSION 0.0.1 DESCRIPTION "Simple library to play chess games")
add_library(simple-chess-games SHARED
	src/Piece.cpp
	src/GameStage.cpp
	src/Square.cpp
	src/Board.cpp
	src/details/fen/FenParser.cpp
	src/details/fen/FenUtils.cpp
	src/details/moves/KnightMove.cpp
	src/details/moves/KingMove.cpp
	src/details/moves/BishopMove.cpp
	src/details/moves/RookMove.cpp
	src/details/moves/PawnMove.cpp
	src/details/moves/QueenMove.cpp
	src/details/DrawEvaluator.cpp
	src/details/MoveValidator.cpp
	src/details/BoardAnalyzer.cpp
	src/PieceMove.cpp
	src/PlayedMove.cpp
	src/Exceptions.cpp
	src/Game.cpp
	src/Color.cpp
	)

set(COMMON_FLAGS -std=c++11 -Wall -Wextra -Wpedantic -Werror)
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

install(TARGETS simple-chess-games
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
