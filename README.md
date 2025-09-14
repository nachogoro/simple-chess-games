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

## Building

### Quick Start (Linux)

While the library is platform-agnostic, the build scripts are designed for
Linux environments:

```bash
git clone https://github.com/nachogoro/simple-chess-games.git
cd simple-chess-games

# Linux native build (Release with tests)
./build-linux.sh
```

### Build Options

```bash
# Linux build with options
./build-linux.sh [Debug|Release] [ON|OFF]
#                 ^build mode    ^tests

# Examples:
./build-linux.sh Release ON     # Release build with tests (default)
./build-linux.sh Debug OFF      # Debug build, libraries only
```

### Manual Build

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON ..
cmake --build . --parallel $(nproc)

# Run tests
./run_c_tests      # C API tests
./run_cpp_tests    # C++ API tests
```

### Build Targets

The build produces four libraries:
- `libsimple-chess-games.so` - C++ shared library
- `libsimple-chess-games-static.a` - C++ static library
- `libsimple-chess-games-c.so` - C shared library
- `libsimple-chess-games-c-static.a` - C static library

### Cross-Compilation

For Windows and Android cross-compilation from Linux:

#### Windows (MinGW-w64)
```bash
# Install prerequisites
sudo apt install mingw-w64 gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64

# Build for Windows
./build-windows.sh Release
```

#### Android
```bash
# Set Android NDK path
export ANDROID_NDK_ROOT="/path/to/android-ndk"

# Build for Android (all architectures)
./build-android.sh Release

# ... or build only specific architectures
./build-android.sh Release arm64-v8a,x86_64
```

#### Build All Platforms
```bash
# Build all platforms in Release mode
./build-all.sh

# Build specific platforms
./build-all.sh Release linux,windows
./build-all.sh Release android:arm64-v8a,x86_64
```

Cross-compiled libraries are output to `dist/lib/{platform}/{arch}/` with shared headers in `dist/include/`.

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

Run the test suite with:
```bash
# Linux build with tests (both C and C++ APIs)
./build-linux.sh

# Manual test execution
cd build-linux
LD_LIBRARY_PATH=. ./run_c_tests      # C API tests
LD_LIBRARY_PATH=. ./run_cpp_tests    # C++ API tests
```

The library includes comprehensive tests for all chess rules, edge cases, and both API implementations to ensure full functionality parity.

## Dependencies

Automatically managed dependencies:
- **Boost**: String algorithms, bidirectional maps, optional I/O
- **GoogleTest**: Unit testing framework (development only)

All dependencies are downloaded and built automatically during the CMake build process.

## License

MIT License - see LICENSE file for details.
