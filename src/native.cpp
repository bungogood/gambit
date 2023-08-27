#include <stdio.h>
#include <string.h>

#include "chess.hpp"

int main() {
    Chess chess;
    Search_Info search_info[1];

    printf(";----------------------------------------------------------;\n");
    printf(";                    nibble-chess v1.0                     ;\n");
    printf(";----------------------------------------------------------;\n");
    printf(";         A tribute to chess programming community         ;\n");
    printf(";  based on the ideas taken from micro-Max by H.G.Muller   ;\n");
    printf(";----------------------------------------------------------;\n");
    printf(";                     by Maksim Korzh;                     ;\n");
    printf(";----------------------------------------------------------;\n");

    printf("\nenter search depth\n( 2 - 6 recommended)\n");

    char move_string[6];
    int side = 8, en_passant_square = 128, depth = getchar() - '0';

    printf("\nEnter move in format:\n\n");
    printf(" e2e4 - common move\n");
    printf("g7g8r - pawn promotin\n");
    printf(" e1g1 - castling\n\n");

    chess.print_board();  // print board

    while (1) {  // game loop
        memset(&move_string[0], 0, sizeof(move_string));

        if (!fgets(move_string, 6, stdin)) continue;
        if (move_string[0] == '\n') continue;

        Move move = chess.parse_move(side, en_passant_square,
                                     move_string);  // parse move

        if (!move.source_square && !move.target_square &&
            !move.promoted_piece) {
            printf("illegal move\n");
            continue;
        }

        chess.make_move(side, move);
        side = 24 - side;
        en_passant_square = move.skip_square;  // make move, update side/e.p.
        chess.print_board();                   // print board

        int score =
            chess.search_position(side, en_passant_square, -10000, 10000, depth,
                                  search_info);  // search position
        printf("\nScore: %d\n\n", score);

        if (score == 10000 || score == -10000) {  // mate
            chess.make_move(side, search_info->best_move);
            side = 24 - side;
            en_passant_square = search_info->best_move.skip_square;
            chess.print_board();
            (score == 10000) ? printf("\nWhite is checkmated!\n")
                             : printf("\nBlack is checkmated!\n");
            break;
        }

        chess.make_move(side, search_info->best_move);
        side = 24 - side;
        en_passant_square =
            search_info->best_move.skip_square;  // make engine's move
        chess.print_board();                     // print board
    }

    return 0;
}
