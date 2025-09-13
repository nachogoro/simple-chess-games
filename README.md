# Simple Chess Games

A C++ chess library that implements chess game logic, move validation, and game
state management.

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

### Technical Features
- Immutable game objects
- Exception-based error handling
- Modern C++17 implementation
- Both shared and static library builds

## Design Principles

The library values:
- **Immutability**: Game objects are immutable, operations return new game
  states
- **Performance**: Efficient move generation and board representation
- **Standards Compliance**: Implements FIDE chess rules accurately

The library is thoroughly tested, with unit tests covering all functionality.

## Requirements

- C++ compiler with C++17 support
- CMake 3.14 or higher
- Linux, macOS, or Windows

No manual dependency installation required - the build system automatically
fetches Boost and GoogleTest.

## Building

### Quick Start

```bash
git clone <repository-url>
cd simple-chess-games

# Build the library
./build.sh

# Or, if you want to also run the tests...
./build-and-run-tests.sh
```

### Manual Build

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run tests (optional)
./run_tests
```

### Build Targets

- `simple-chess-games`: Shared library
- `simple-chess-games-static`: Static library
- `run_tests`: Test executable

## API Overview

### Core Classes

- `GameManager`: Entry point for creating and manipulating games
- `Game`: Immutable representation of a chess game state
- `PieceMove`: Represents chess moves (regular, castling, en passant, promotion)
- `Square`: Chess board square representation
- `Piece`: Chess piece with type and color
- `GameStage`: Single game position with complete state information

### Error Handling

The library uses exceptions for error conditions:
- `std::invalid_argument`: Invalid input (malformed FEN strings, etc.)
- `IllegalStateException`: Invalid game operations (moves in finished games, etc.)

## Usage

For usage examples, see the test files in the `tests/` directory. These demonstrate all library functionality including:
- Game creation and move making
- FEN parsing and generation
- Draw detection and claiming
- Special move handling
- Game state queries

## Testing

Run the test suite with:
```bash
./build-and-run-tests.sh
```

The library includes comprehensive tests for all chess rules, edge cases, and API functionality.

## Dependencies

Automatically managed dependencies:
- **Boost**: String algorithms, bidirectional maps, optional I/O
- **GoogleTest**: Unit testing framework (development only)

All dependencies are downloaded and built automatically during the CMake build process.

## License

MIT License - see LICENSE file for details.
