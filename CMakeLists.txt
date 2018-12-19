cmake_minimum_required(VERSION 3.9)
project(simple-chess-games VERSION 0.0.1 DESCRIPTION "Simple library to play chess games")
add_library(simple-chess-games SHARED
	src/Board.cpp
	src/BoardUtils.cpp
	src/BoardAnalyzer.cpp
	src/GameEngine.cpp
	src/GameEngineImpl.cpp
	src/Game.cpp
	src/GameImpl.cpp
	src/MoveMap.cpp
	src/MoveHistory.cpp
	src/movebehaviour/MoveBehaviour.cpp
	src/movebehaviour/MoveBehaviourPawn.cpp
	src/movebehaviour/MoveBehaviourRook.cpp
	src/movebehaviour/MoveBehaviourKnight.cpp
	src/movebehaviour/MoveBehaviourBishop.cpp
	src/movebehaviour/MoveBehaviourQueen.cpp
	src/movebehaviour/MoveBehaviourKing.cpp
	)

set(COMMON_FLAGS -std=c++11 -Wall -Wextra -Wpedantic -Werror)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_target_properties(simple-chess-games PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER include/Board.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER include/Exception.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER include/GameEngine.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER include/Game.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER include/Move.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER include/Piece.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER include/PossibleMove.h)
set_target_properties(simple-chess-games PROPERTIES PUBLIC_HEADER include/Square.h)
target_include_directories(simple-chess-games PRIVATE include)
target_include_directories(simple-chess-games PRIVATE src)

include(GNUInstallDirs)

install(TARGETS simple-chess-games
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
