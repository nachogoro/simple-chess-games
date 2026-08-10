cmake_minimum_required(VERSION 3.14)
project(simple-chess-games VERSION 0.0.1 DESCRIPTION "Simple library to play chess games")

# Core chess engine sources
set(core_sources
	src/core/Board.cpp
	src/core/Builders.cpp
	src/core/Color.cpp
	src/core/Exceptions.cpp
	src/core/Game.cpp
	src/core/GameStage.cpp
	src/core/Piece.cpp
	src/core/PieceMove.cpp
	src/core/PlayedMove.cpp
	src/core/SimpleChess.cpp
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

# C interface sources
set(c_interface_sources
	src/c_interface/simplechess.cpp
	src/c_interface/conversion_utils.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Common warning flags for all build types
set(COMMON_WARNING_FLAGS -Wall -Wextra -Wpedantic -Werror)

# Configure compiler flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug: No optimizations, debug symbols
    set(COMMON_FLAGS ${COMMON_WARNING_FLAGS} -g -O0)
    message(STATUS "Debug build: No optimizations, debug symbols enabled")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Release: Maximum optimizations for runtime performance
    set(COMMON_FLAGS ${COMMON_WARNING_FLAGS} -O3 -DNDEBUG)
    message(STATUS "Release build: Optimized for runtime performance")
else()
    # Fallback for other build types
    set(COMMON_FLAGS ${COMMON_WARNING_FLAGS})
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

# Enable fPIC for all targets (required for shared libraries)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Nothing is visible in a shared build unless it is marked SIMPLECHESS_API,
# so that src/core/details stays out of what a program links against and
# every platform exports the same set.
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

include(GNUInstallDirs)

# ===== OBJECT LIBRARIES (compiled once, reused) =====

# The same sources are compiled more than once, because what a translation
# unit exports depends on which library it ends up in:
#
#   cpp-shared  the shared C++ library, which exports the C++ API
#   c-shared    the shared C library, which carries the engine so as to be
#               self-contained but exports only the C API
#   static      the static libraries, which export nothing
foreach(kind cpp-shared c-shared static)
	add_library(core-objects-${kind} OBJECT ${core_sources})
	target_include_directories(core-objects-${kind} PUBLIC include)
	target_link_libraries(core-objects-${kind} PRIVATE Boost::algorithm)
	target_compile_options(core-objects-${kind} PRIVATE ${COMMON_FLAGS})
endforeach()

foreach(kind c-shared static)
	add_library(c-interface-objects-${kind} OBJECT ${c_interface_sources})
	target_include_directories(c-interface-objects-${kind} PUBLIC include)
	target_link_libraries(c-interface-objects-${kind} PRIVATE Boost::algorithm)
	target_compile_options(c-interface-objects-${kind} PRIVATE ${COMMON_FLAGS})
endforeach()

target_compile_definitions(core-objects-cpp-shared PRIVATE SIMPLECHESS_BUILDING)
target_compile_definitions(core-objects-c-shared PRIVATE SIMPLECHESS_C_BUILDING)
target_compile_definitions(c-interface-objects-c-shared PRIVATE SIMPLECHESS_C_BUILDING)
target_compile_definitions(core-objects-static PUBLIC SIMPLECHESS_STATIC)
target_compile_definitions(c-interface-objects-static PUBLIC SIMPLECHESS_STATIC)

# ===== C++ LIBRARY =====

# Boost is wrapped in BUILD_INTERFACE on the libraries below. Only its
# header-only algorithm library is used, and only inside implementation
# files, so an installed archive needs nothing from it - but a static
# library passes even its PRIVATE dependencies on to whoever links it, and
# exporting one which is not itself exported is an error.

# Shared C++ library
add_library(simple-chess-games SHARED $<TARGET_OBJECTS:core-objects-cpp-shared>)
add_library(SimpleChess::simple-chess-games ALIAS simple-chess-games)
target_include_directories(simple-chess-games PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(simple-chess-games PRIVATE $<BUILD_INTERFACE:Boost::algorithm>)

# Static C++ library
add_library(simple-chess-games-static STATIC $<TARGET_OBJECTS:core-objects-static>)
add_library(SimpleChess::simple-chess-games-static ALIAS simple-chess-games-static)
target_include_directories(simple-chess-games-static PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_definitions(simple-chess-games-static PUBLIC SIMPLECHESS_STATIC)
target_link_libraries(simple-chess-games-static PRIVATE $<BUILD_INTERFACE:Boost::algorithm>)

# The headers are installed as whole directories further down, so no target
# lists them: PUBLIC_HEADER installs them flattened into the include prefix,
# which both collides with the rest of the system and breaks the paths the
# headers use on each other.
set_target_properties(simple-chess-games PROPERTIES
	VERSION ${PROJECT_VERSION})

set_target_properties(simple-chess-games-static PROPERTIES
	VERSION ${PROJECT_VERSION})

# ===== C LIBRARY =====

# Shared C library (includes both core and C interface)
add_library(simple-chess-games-c SHARED $<TARGET_OBJECTS:core-objects-c-shared> $<TARGET_OBJECTS:c-interface-objects-c-shared>)
add_library(SimpleChess::simple-chess-games-c ALIAS simple-chess-games-c)
target_include_directories(simple-chess-games-c PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(simple-chess-games-c PRIVATE $<BUILD_INTERFACE:Boost::algorithm>)

# Static C library
add_library(simple-chess-games-c-static STATIC $<TARGET_OBJECTS:core-objects-static> $<TARGET_OBJECTS:c-interface-objects-static>)
add_library(SimpleChess::simple-chess-games-c-static ALIAS simple-chess-games-c-static)
target_include_directories(simple-chess-games-c-static PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_definitions(simple-chess-games-c-static PUBLIC SIMPLECHESS_STATIC)
target_link_libraries(simple-chess-games-c-static PRIVATE $<BUILD_INTERFACE:Boost::algorithm>)

set_target_properties(simple-chess-games-c PROPERTIES
	VERSION ${PROJECT_VERSION})

set_target_properties(simple-chess-games-c-static PROPERTIES
	VERSION ${PROJECT_VERSION})

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# GoogleTest brings its own install rules, which would otherwise put the test
# framework - headers, archives, CMake config and all - into the prefix this
# library is installed to.
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Boost
FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.89.0/boost-1.89.0-cmake.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(BOOST_ENABLE_CMAKE ON)
# Only build the libraries we need
set(BOOST_INCLUDE_LIBRARIES algorithm optional)
FetchContent_MakeAvailable(Boost)

# Install targets
install(TARGETS
		simple-chess-games simple-chess-games-c
		simple-chess-games-static simple-chess-games-c-static
	EXPORT simple-chess-gamesTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# So that a consumer can find_package(simple-chess-games) and link
# SimpleChess::simple-chess-games, rather than naming the library file and
# setting include paths by hand.
install(EXPORT simple-chess-gamesTargets
	FILE simple-chess-gamesTargets.cmake
	NAMESPACE SimpleChess::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simple-chess-games)

include(CMakePackageConfigHelpers)

configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/simple-chess-gamesConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/simple-chess-gamesConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simple-chess-games)

write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/simple-chess-gamesConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion)

install(FILES
		${CMAKE_CURRENT_BINARY_DIR}/simple-chess-gamesConfig.cmake
		${CMAKE_CURRENT_BINARY_DIR}/simple-chess-gamesConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simple-chess-games)

# Install directory structure for headers
install(DIRECTORY include/simplechess DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/simplechess-c DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Option to build tests (enabled by default)
option(BUILD_TESTS "Build test executables" ON)

if(BUILD_TESTS)
    message(STATUS "Building tests enabled")
    enable_testing()
else()
    message(STATUS "Building tests disabled - skipping for faster compilation")
endif()

if(BUILD_TESTS)
    # C++ Tests
    add_executable(run_cpp_tests
        tests/cpp/AlgebraicNotation_test.cpp
        tests/cpp/DrawDetection_test.cpp
        tests/cpp/FenGeneration_test.cpp
        tests/cpp/GameCreation_test.cpp
        tests/cpp/MoveAvailability_test.cpp
        tests/cpp/MoveCounter_test.cpp
        tests/cpp/MovesOnBoard_test.cpp
        tests/cpp/Perft_test.cpp
        tests/cpp/RepetitionKey_test.cpp
        tests/cpp/Resignation_test.cpp)
    target_include_directories(run_cpp_tests PRIVATE include)
    target_include_directories(run_cpp_tests PRIVATE src/core)
    target_include_directories(run_cpp_tests PRIVATE tests/cpp)
    # Perft_test and RepetitionKey_test reach into src/core/details, which is
    # not part of the installed library, so link the object files directly.
    target_link_libraries(run_cpp_tests gtest_main simple-chess-games-static Boost::optional Boost::algorithm)

    # C Interface Tests
    add_executable(run_c_tests
        tests/c/AlgebraicNotation_test.cpp
        tests/c/GameCreation_test.cpp
        tests/c/FenGeneration_test.cpp
        tests/c/DrawDetection_test.cpp
        tests/c/Resignation_test.cpp
        tests/c/MoveCounter_test.cpp
        tests/c/MoveAvailability_test.cpp
        tests/c/MovesOnBoard_test.cpp
        tests/c/Errors_test.cpp
        tests/c/ValueHelpers_test.cpp
        # The only translation unit in the project compiled as C. It exists
        # so that the C headers are checked against a C compiler, which the
        # rest of this suite cannot do: GoogleTest needs C++.
        tests/c/c_header_compile_check.c)
    target_include_directories(run_c_tests PRIVATE include)
    target_include_directories(run_c_tests PRIVATE tests/c)
    target_link_libraries(run_c_tests gtest_main simple-chess-games-c)
    set_source_files_properties(tests/c/c_header_compile_check.c PROPERTIES
        COMPILE_OPTIONS "-std=c99;-Wall;-Wextra;-Wpedantic;-Werror")

    include(GoogleTest)
    gtest_discover_tests(run_cpp_tests)
    gtest_discover_tests(run_c_tests)

    # Backward compatibility - run_tests now runs C++ tests
    add_custom_target(run_tests DEPENDS run_cpp_tests)
endif()

# Option to build benchmarks (disabled by default; never installed)
option(BUILD_BENCHMARKS "Build the benchmark driver" OFF)

if(BUILD_BENCHMARKS)
    message(STATUS "Building benchmarks enabled")
    add_executable(bench_simplechess benchmarks/bench_main.cpp)
    target_include_directories(bench_simplechess PRIVATE include)
    # The C static library bundles the core objects too, so this one target
    # covers both the C++ and the C workloads.
    target_link_libraries(bench_simplechess simple-chess-games-c-static Boost::algorithm)
    target_compile_options(bench_simplechess PRIVATE ${COMMON_FLAGS})
endif()
