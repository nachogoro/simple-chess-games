# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

### Standard Build and Test
```bash
# Build the project
./build.sh

# Build and run C tests (default)
./build-and-run-tests.sh

# Build and run C++ tests
./cpp-build-and-run.sh
```

### Manual Build
```bash
# Standard CMake build
cmake -S . -B build && cmake --build build

# Run specific test suites
cd build
./run_c_tests      # C interface tests
./run_cpp_tests    # C++ API tests
```

### Single Test Execution
```bash
# Run specific test filters (GoogleTest syntax)
cd build
LD_LIBRARY_PATH=build ./run_c_tests --gtest_filter="*AlgebraicNotation*"
LD_LIBRARY_PATH=build ./run_cpp_tests --gtest_filter="GameCreation*"
```

## Project Architecture

### Dual API Design
This project provides **two complete APIs** for the same chess engine:

1. **C++ API** (`include/cpp/simplechess/`) - Modern C++17 interface with exceptions
2. **C API** (`include/c/simplechess/`) - C-compatible interface returning NULL on errors

Both APIs expose identical functionality but follow their respective language conventions.

### Core Architecture Layers

#### 1. Core Engine (`src/core/`)
- **Game Logic**: `Game.cpp`, `GameStage.cpp` - Immutable game state management
- **Board Representation**: `Board.cpp`, `Square.cpp`, `Piece.cpp` - Chess board modeling
- **Move System**: `PieceMove.cpp`, `PlayedMove.cpp` - Move representation and history
- **Rule Engine**: `src/core/details/` - Chess rules, validation, and move generation
  - `MoveValidator.cpp` - Legal move validation
  - `GameStateDetector.cpp` - Checkmate/stalemate detection
  - `DrawEvaluator.cpp` - Draw conditions (50-move rule, repetition, etc.)
  - `AlgebraicNotationGenerator.cpp` - Standard chess notation

#### 2. Move Generation (`src/core/details/moves/`)
Piece-specific move logic with files for each piece type:
- `PawnMove.cpp`, `RookMove.cpp`, `BishopMove.cpp`, `KnightMove.cpp`, `KingMove.cpp`, `QueenMove.cpp`

#### 3. FEN Support (`src/core/details/fen/`)
- `FenParser.cpp` - Parse Forsyth-Edwards Notation
- `FenUtils.cpp` - FEN string generation and utilities

#### 4. C Interface Layer (`src/c_interface/`)
- `simplechess.cpp` - C API implementation wrapping C++ core
- `conversion_utils.cpp` - Type conversion between C and C++ representations

### Library Outputs
The build produces **four libraries**:
- `libsimple-chess-games.so` - C++ shared library
- `libsimple-chess-games-static.a` - C++ static library
- `libsimple-chess-games-c.so` - C shared library (includes both core + C interface)
- `libsimple-chess-games-c-static.a` - C static library

### Key Design Principles

#### Immutability
All game operations return **new game states** rather than modifying existing ones. This is fundamental to the API design:
```cpp
// C++ API
Game newGame = makeMove(currentGame, move);

// C API
game_t* newGame = simple_chess_make_move(currentGame, move);
```

#### Exception vs Error Code Handling
- **C++ API**: Uses exceptions (`IllegalStateException`, `std::invalid_argument`)
- **C API**: Returns `NULL` pointers for error conditions

#### Memory Management
- **C++ API**: Automatic RAII memory management
- **C API**: Manual memory management - caller must call `destroy_game()` on all returned game objects

### Test Structure
- `tests/cpp/` - C++ API tests using GoogleTest
- `tests/c/` - C API tests using GoogleTest (tests C interface wrapping)

Both test suites cover identical functionality to ensure API parity.

### Dependencies
- **Boost**: Algorithm library (string utilities), bimap (bidirectional maps)
- **GoogleTest**: Testing framework
- All dependencies auto-downloaded via CMake FetchContent

### Current Branch Context
Working on branch `c-interface` which implements and tests the C API wrapper around the existing C++ chess engine.