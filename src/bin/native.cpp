#include <stdio.h>
#include <string.h>

#include "chess.hpp"
#include "fsm.hpp"

// void play() {
//     Chess chess;
//     Search_Info search_info[1];

//     printf(";----------------------------------------------------------;\n");
//     printf(";                    nibble-chess v1.0                     ;\n");
//     printf(";----------------------------------------------------------;\n");
//     printf(";         A tribute to chess programming community         ;\n");
//     printf(";  based on the ideas taken from micro-Max by H.G.Muller   ;\n");
//     printf(";----------------------------------------------------------;\n");
//     printf(";                     by Maksim Korzh;                     ;\n");
//     printf(";----------------------------------------------------------;\n");

//     printf("\nenter search depth\n( 2 - 6 recommended)\n");

//     char move_string[6];
//     int side = WHITE, en_passant_square = 128, depth = getchar() - '0';

//     printf("\nEnter move in format:\n\n");
//     printf(" e2e4 - common move\n");
//     printf("g7g8r - pawn promotin\n");
//     printf(" e1g1 - castling\n\n");

//     chess.print_board();  // print board

//     while (1) {  // game loop
//         memset(&move_string[0], 0, sizeof(move_string));

//         if (!fgets(move_string, 6, stdin)) continue;
//         if (move_string[0] == '\n') continue;

//         Move move = chess.parse_move(move_string, side,
//                                      en_passant_square);  // parse move

//         if (!move.source_square && !move.target_square &&
//             !move.promoted_piece) {
//             printf("illegal move\n");
//             continue;
//         }

//         chess.make_move(move, side);
//         side = 24 - side;
//         en_passant_square = move.skip_square;  // make move, update side/e.p.
//         chess.print_board();                   // print board

//         int score =
//             chess.search_position(side, en_passant_square, -10000, 10000,
//             depth,
//                                   search_info);  // search position
//         printf("\nScore: %d\n\n", score);

//         if (score == 10000 || score == -10000) {  // mate
//             chess.make_move(search_info->best_move, side);
//             side = 24 - side;
//             en_passant_square = search_info->best_move.skip_square;
//             chess.print_board();
//             (score == 10000) ? printf("\nWhite is checkmated!\n")
//                              : printf("\nBlack is checkmated!\n");
//             break;
//         }

//         chess.make_move(search_info->best_move, side);
//         side = 24 - side;
//         en_passant_square =
//             search_info->best_move.skip_square;  // make engine's move
//         chess.print_board();                     // print board
//     }
// }

#include <iostream>

int main() {
    // variable setup
    Chess chess;
    enum FSMState state = Idle;
    Move_List move_list[1];
    Move_List current_square_moves[1];
    State_Memory state_memory[1];

    std::cout << "hello world" << std::endl;
    std::cout << "Initial state: " << state << std::endl;

    print_bitboard(chess.get_occupied());
    std::cout << std::endl;
    int side = 8;  // 8 = white, 16 = black
    while (true) {
        std::cout << "Generating moves..." << std::endl;
        chess.generate_moves(move_list, 0);
        chess.print_board();
        while (true) {
            int square;
            std::cout << "Type number of square: " << std::endl;
            std::cin >> square;
            std::cout << "The square is " << square << std::endl;
            state = update_state(&chess, square, state, state_memory,
                                 move_list, current_square_moves);
            std::cout << "Current state is: " << state << std::endl;
            std::cout << "FSMState memory 0: " << state_memory->memory[0] << std::endl;
            std::cout << "FSMState memory 1: " << state_memory->memory[1] << std::endl;
            if (state == FSMState::MoveComplete) break;
        }
        state = FSMState::Idle;
        std::cout << "Move complete" << std::endl;
        state_memory->length = 0;
        side = 24 - side;
    }
}