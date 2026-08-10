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
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON
cmake --build build --parallel $(nproc)

# Run tests
./build/run_c_tests      # C API tests
./build/run_cpp_tests    # C++ API tests
```

The build produces four libraries:
- `libsimple-chess-games.so` - C++ shared library
- `libsimple-chess-games-static.a` - C++ static library
- `libsimple-chess-games-c.so` - C shared library
- `libsimple-chess-games-c-static.a` - C static library

Each is self-contained: the C libraries carry their own copy of the engine
rather than depending on the C++ ones.

## Using the library

Install it, then find it from CMake:

```bash
cmake --install build --prefix /usr/local
```

```cmake
find_package(simple-chess-games REQUIRED)

target_link_libraries(my_app PRIVATE SimpleChess::simple-chess-games)
# or SimpleChess::simple-chess-games-c        for the C API
# or SimpleChess::simple-chess-games-static   to link statically
```

`tests/consumer` is a working example of exactly this, built out of tree
against nothing but an install prefix.

## APIs

Both APIs drive the same engine and offer the same functionality, following
their respective language conventions.

### C++ API (`include/simplechess/`)

A game is immutable: playing a move returns a new game rather than changing
the one it was played on.

```cpp
#include <simplechess/SimpleChess.h>

using namespace simplechess;

Game game = createNewGame();

// A move can be looked up by the two squares it goes between, so a caller
// does not have to work out which piece is standing where.
const std::optional<PieceMove> move = findMove(
        game, Square::fromString("e2"), Square::fromString("e4"));

if (move)
{
    game = makeMove(game, *move);

    std::cout << game.history().back().move.inAlgebraicNotation() << "\n";
}
```

The available moves come out as a `std::vector<PieceMove>`, ordered and free
of duplicates, so a game can also be driven by picking from them directly:

```cpp
while (game.gameState() == GameState::Playing)
{
    const std::vector<PieceMove>& moves = game.allAvailableMoves();
    game = makeMove(game, moves[chooseSomehow(moves.size())]);
}
```

Errors are exceptions, and only the functions which advance a game throw
them:

- `IllegalStateException` - the game has already finished, the move is not
  legal, or no draw can be claimed. Thrown by `makeMove`, `claimDraw` and
  `resign`.
- `InvalidArgumentException` - the value handed over does not describe
  anything the library can work with, such as a malformed FEN string or a
  square outside the board. It derives from `std::invalid_argument`.

Everything which merely reports on a game - `gameState()`, `drawReason()`,
`reasonToClaimDraw()`, `history()` and the rest - never throws. Where an
answer may not exist, it is an empty `std::optional`.

### C API (`include/simplechess-c/`)

A game is an opaque handle: created by a factory function, read through the
`simple_chess_game_*` accessors, and released with
`simple_chess_destroy_game()`.

```c
#include <simplechess-c/simplechess.h>

simple_chess_error_t error;
simple_chess_square_t src;
simple_chess_square_t dst;
simple_chess_piece_move_t move;

simple_chess_game_t* game = simple_chess_create_new_game(
        SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, &error);

if (game == NULL)
{
    fprintf(stderr, "%s\n", simple_chess_error_string(error));
    return 1;
}

simple_chess_square_from_string("e2", &src);
simple_chess_square_from_string("e4", &dst);

if (simple_chess_find_move(
            game, src, dst, false, SIMPLE_CHESS_PIECE_TYPE_PAWN, &move))
{
    /* Playing a move returns a new game; the old one is still yours to
     * destroy. */
    simple_chess_game_t* next = simple_chess_make_move(game, move, &error);

    if (next == NULL)
    {
        fprintf(stderr, "%s\n", simple_chess_error_string(error));
    }
    else
    {
        simple_chess_destroy_game(game);
        game = next;
    }
}

simple_chess_destroy_game(game);
```

Two conventions run through the whole C API:

- The functions which produce a game return it, or `NULL` with the reason
  written to a trailing `simple_chess_error_t*`. That pointer may be `NULL`
  if the caller does not care why; on success it is set to `SIMPLE_CHESS_OK`.
  There is no global error state to read afterwards.
- Every other call which can fail returns `bool` and writes its result
  through an out-parameter, leaving it untouched on failure.

There is no direct counterpart to the C++ `availableMovesForPiece()`; scan
`simple_chess_game_available_move()` for the moves whose `src` is the square
in question.

### Threading

The library is single-threaded. Objects are immutable from the caller's point
of view, but some values are derived only when they are first asked for, so a
given object must not be used from more than one thread at a time. Separate
threads working on separate objects are fine.

### Binary compatibility

The C ABI is stable across 2.x: the game is opaque and every other type is a
plain value, so no layout the library might change is part of the interface.

The C++ ABI is not. The headers expose the private members of every class -
deliberately, since that layout and the inline accessors are what make move
generation fast - so a change inside the library can change them. Programs
using the C++ API should be recompiled against the version they will run
with.

## Usage

The snippets above are a starting point. For worked examples of everything
the library does, see the test files:

- `tests/cpp/` - C++ API examples and tests
- `tests/c/` - C API examples and tests
- `tests/consumer/` - using the installed library from another project

These demonstrate all library functionality including:
- Game creation and move making
- FEN parsing and generation
- Draw detection and claiming
- Special move handling
- Game state queries

## Testing

Run the test suite after building with `-DBUILD_TESTS=ON`:
```bash
./build/run_c_tests      # C API tests
./build/run_cpp_tests    # C++ API tests
```

The library includes comprehensive tests for all chess rules, edge cases, and
both API implementations to ensure full functionality parity.

There is also a benchmark and regression driver, built with
`-DBUILD_BENCHMARKS=ON` and never installed:

```bash
./build/bench_simplechess              # timing workloads
./build/bench_simplechess golden       # deterministic trace of the public API
./build/bench_simplechess crosscheck   # check the two APIs agree, ply by ply
```

`golden` is the safety net for changes which are not meant to alter
behaviour: capture it before and after, and diff. It must be byte-identical.

## Migrating from 1.x

The interface changed in ways which do not keep source compatibility. This
covers everything a program written against v1.0.1 has to change.

**Both APIs**

- The headers moved out of their `cpp/` and `c/` prefixes:
  `<cpp/simplechess/Game.h>` is now `<simplechess/Game.h>`, and
  `<c/simplechess/simplechess.h>` is now `<simplechess-c/simplechess.h>`.

**C++**

- `allAvailableMoves()` and `availableMovesForPiece()` return
  `std::vector<PieceMove>` instead of `std::set<PieceMove>`. The order is
  unchanged: the vector is sorted with the same comparison the set used.
- `history()` returns `std::vector<HistoryEntry>` instead of a vector of
  `std::pair`. `entry.first` and `entry.second` become `entry.stage` and
  `entry.move`.
- `drawReason()` returns `const std::optional<DrawReason>&` and no longer
  throws; `reasonToClaimDraw()` no longer throws either. Both report the
  absence of a reason with an empty optional.
- FEN and square parsing throw `InvalidArgumentException` rather than
  `std::invalid_argument` directly. It derives from `std::invalid_argument`,
  so existing handlers keep working.
- `PieceMove::pawnPromotion()` now rejects the arguments it always
  documented as invalid.
- `findMove()` is new.

**C**

- Types are prefixed and enumerators are screaming-snake: `game_t` became
  `simple_chess_game_t`, `ColorWhite` became `SIMPLE_CHESS_COLOR_WHITE`.
- `simple_chess_game_t` is opaque. Field reads become accessor calls:
  `game->state` becomes `simple_chess_game_state(game)`, and
  `game->current_stage` becomes
  `simple_chess_game_current_stage(game, &stage)`.
- Every function which can fail takes a trailing `simple_chess_error_t*`.
  Passing `NULL` for it keeps the old behaviour of only knowing that
  something went wrong.
- `destroy_game()` became `simple_chess_destroy_game()`.
- The `_ex` factories are gone; the plain ones take the draw enforcement
  mode.
- `simple_chess_board_t` holds one `squares[64]` of
  `simple_chess_square_content_t` rather than a pair of parallel arrays.
  `simple_chess_square_content_piece()` unpacks one into a piece.
- `simple_chess_square_from_index()` and `simple_chess_index_from_square()`
  report failure instead of being undefined on input off the board.
- `simple_chess_find_move()`, `simple_chess_piece_move_equals()`,
  `simple_chess_square_from_string()`, `simple_chess_square_to_string()`,
  `simple_chess_square_is_valid()`, `simple_chess_square_color()` and
  `simple_chess_copy_game()` are new.

## Dependencies

Automatically managed dependencies:
- **Boost**: String algorithms. Header-only and an implementation detail -
  nothing a consumer has to find or link against
- **GoogleTest**: Unit testing framework (development only)

All dependencies are downloaded and built automatically during the CMake build process.

## License

MIT License - see LICENSE file for details.
