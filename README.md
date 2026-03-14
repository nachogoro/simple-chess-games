# Simple Chess Games

A C++ chess library that implements game logic, move validation, and game
state management.

The library provides two complete APIs: a modern C++17 interface focused on
immutability and a C-compatible interface for broader language interoperability.

Both APIs include thorough test suites.

## Features

### Basic Functionality
- Complete chess rule implementation including castling, en passant, and pawn
  promotion
- Move validation for all piece types
- Game state detection (checkmate, stalemate, draw conditions)
- FEN (Forsyth-Edwards Notation) support for game creation and export
- Algebraic notation generation for moves
- Game history tracking with complete move sequences

### Draw Detection
- Stalemate
- Insufficient material
- Threefold and fivefold repetition
- Fifty-move and seventy-five-move rules
- Draw offers and claims

## Requirements

- C++ compiler with C++17 support
- CMake 3.14 or higher
- Linux, macOS, or Windows

No manual dependency installation required - the build system automatically
fetches Boost and GoogleTest.

## Pre-built Binaries

Pre-built libraries for all supported platforms are available from
[GitHub Releases](https://github.com/nachogoro/simple-chess-games/releases).
Binaries are built automatically via CI for Linux, Windows, macOS (arm64 and
x86_64), and Android (arm64-v8a, armeabi-v7a, x86_64, x86).

## Building from Source

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON ..
cmake --build . --parallel $(nproc)

# Run tests
./run_c_tests      # C API tests
./run_cpp_tests    # C++ API tests
```

The build produces four libraries:
- `libsimple-chess-games.so` - C++ shared library
- `libsimple-chess-games-static.a` - C++ static library
- `libsimple-chess-games-c.so` - C shared library
- `libsimple-chess-games-c-static.a` - C static library

## APIs

This library provides two complete APIs for the same chess engine:

### C++ API (`include/cpp/simplechess/`)
Modern C++17 interface with exception-based error handling:
```cpp
#include "cpp/simplechess/Game.h"

Game game = createGame();
Game newGame = makeMove(game, move);  // Returns new game state
```

### C API (`include/c/simplechess/`)
C-compatible interface with NULL return codes for errors:
```c
#include "c/simplechess/simplechess.h"

game_t* game = simple_chess_create_game();
game_t* newGame = simple_chess_make_move(game, move);
if (newGame == NULL) { /* handle error */ }
simple_chess_destroy_game(game);  // Manual memory management required
```

Both APIs expose identical functionality but follow their respective language conventions.

## Usage

For usage examples, see the test files in the `tests/` directory:
- `tests/cpp/` - C++ API examples and tests
- `tests/c/` - C API examples and tests

These demonstrate all library functionality including:
- Game creation and move making
- FEN parsing and generation
- Draw detection and claiming
- Special move handling
- Game state queries

## Testing

Run the test suite after building with `-DBUILD_TESTS=ON`:
```bash
cd build
./run_c_tests      # C API tests
./run_cpp_tests    # C++ API tests
```

The library includes comprehensive tests for all chess rules, edge cases, and
both API implementations to ensure full functionality parity.

## Dependencies

Automatically managed dependencies:
- **Boost**: String algorithms, bidirectional maps, optional I/O
- **GoogleTest**: Unit testing framework (development only)

All dependencies are downloaded and built automatically during the CMake build process.

## License

MIT License - see LICENSE file for details.
