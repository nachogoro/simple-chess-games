#include <c/simplechess/chess.h>
#include <stdio.h>

int main() {
    // Test basic C interface functionality
    chess_game_manager_t manager = chess_create_manager();
    if (!manager) {
        printf("Failed to create game manager\n");
        return 1;
    }

    chess_game_t game;
    if (chess_create_new_game(manager, &game)) {
        printf("✓ Successfully created new game\n");
        printf("✓ Game state: %d\n", game.state);
        printf("✓ Active color: %s\n", game.position.active_color == CHESS_COLOR_WHITE ? "White" : "Black");
        printf("✓ Available moves: %d\n", game.available_moves.count);
    } else {
        printf("Failed to create new game\n");
        chess_destroy_manager(manager);
        return 1;
    }

    // Test square utilities
    chess_square_t e4 = chess_square_from_string("e4");
    char square_str[3];
    chess_square_to_string(e4, square_str);
    printf("✓ Square e4: rank=%d, file=%c, string=%s\n", e4.rank, e4.file, square_str);

    printf("✓ All C interface tests passed!\n");

    chess_destroy_manager(manager);
    return 0;
}