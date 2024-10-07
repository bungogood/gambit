#include "fsm.hpp"

#include <stdio.h>
#include <string.h>

// #include <iostream>
#include <Arduino.h>

#include "chess.hpp"

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                                Helper Functions ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

uint64_t get_bitboard_from_square(int square) {
    int mod = square % 8;
    int div = square / 8;
    return 1ull << (63 - ((div / 2 * 8) + mod));
}

void get_moves_from_square(Move_List *move_list,
                           Move_List *current_square_moves, int square) {
    Serial.println("SQUARE: " + String(square) +
                   " MOVE LIST LENGTH: " + String(move_list->length));
    current_square_moves->length = 0;
    for (int i = 0; i < move_list->length; i++) {
        // std::cout << "SEARCHING " << i << std::endl;
        Serial.println("OPTION: " + String(move_list->moves[i].source_square));
        if (move_list->moves[i].source_square == square) {
            Serial.println(move_list->moves[i].target_square);

            // std::cout << "FOUND" << std::endl;
            // std::cout << "Target square: " <<
            // move_list->moves[i].target_square
            //           << std::endl;
            current_square_moves->moves[current_square_moves->length] =
                move_list->moves[i];
            current_square_moves->length++;
        }
    }
    // std::cout << "Searching complete, found " << current_square_moves->length
    //           << " moves" << std::endl;
}

void get_attacks_on_square(Move_List *move_list,
                           Move_List *current_square_moves, int square) {
    current_square_moves->length = 0;
    for (int i = 0; i < move_list->length; i++) {
        // std::cout << "SEARCHING " << i << std::endl;
        if (move_list->moves[i].target_square == square) {
            // std::cout << "FOUND" << std::endl;
            // std::cout << "Source square: " <<
            // move_list->moves[i].source_square
            //           << std::endl;
            current_square_moves->moves[current_square_moves->length] =
                move_list->moves[i];
            current_square_moves->length++;
        }
    }
    // std::cout << "Searching complete, found " << current_square_moves->length
    //           << " moves" << std::endl;
}

bool is_friendly(Chess *chess, int square) {
    int piece = chess->get_piece_on_square(square);
    if (chess->get_side() == WHITE) {
        return piece > 0 && piece < 18 || piece == 43 || piece == 46;
    } else {
        return piece >= 18 && piece < 24 || piece == 51 || piece == 54;
    }
}

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                                Actual Functions ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

void show_uint64(uint64_t value) {
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0) {
            Serial.println();
        }
        Serial.print((value & (1ull << (63 - i))) ? "1" : "0");
    }
    Serial.println();
}

FSMState update_state(Chess *chess, int instruction, FSMState state,
                      State_Memory *state_memory, Move_List *move_list,
                      Move_List *current_square_moves) {
    // uint64_t square = get_bitboard_from_square(instruction);
    int sq8x8 = to_index(instruction);
    Serial.println("Index: " + String(sq8x8));
    uint64_t square = 1ULL << sq8x8;
    uint64_t occupied = chess->get_occupied();
    uint64_t friendlies;
    uint64_t enemies;

    if (chess->get_side() == WHITE) {  // white turn
        friendlies = chess->get_white();
        enemies = chess->get_black();
    } else {  // black turn
        friendlies = chess->get_black();
        enemies = chess->get_white();
        Serial.println("BLACK TURN");
        show_uint64(square);
        show_uint64(friendlies);
        show_uint64(enemies);
    }

    switch (state) {
        case Idle:
            Serial.println(friendlies & square);
            if ((friendlies & square) != 0ULL) {
                get_moves_from_square(move_list, current_square_moves,
                                      instruction);
                state_memory->memory[0] = instruction;
                state_memory->length = 1;
                return FSMState::FriendlyPU;
            } else if ((enemies & square) != 0ULL) {
                get_attacks_on_square(move_list, current_square_moves,
                                      instruction);
                state_memory->memory[0] = instruction;
                state_memory->length = 1;
                if (current_square_moves->length > 0) {
                    return FSMState::EnemyPU;
                } else {
                    return FSMState::InvalidPiecePU;
                }
            } else {
                return FSMState::Error;
            }
            break;
        case FriendlyPU:
            if (instruction == state_memory->memory[0]) {
                state_memory->length--;
                return FSMState::Idle;
            } else if (chess->get_piece_on_square(instruction) == 43 ||
                       chess->get_piece_on_square(instruction) == 51) {  // king
                state_memory->memory[1] = instruction;
                state_memory->length++;
                return FSMState::Castling;

            } else if (chess->get_piece_on_square(instruction) == 46 ||
                       chess->get_piece_on_square(instruction) == 54) {  // rook
                state_memory->memory[1] = instruction;
                state_memory->length++;
                return FSMState::Castling;

            } else if ((friendlies & square) != 0ULL) {
                state_memory->memory[1] = instruction;
                state_memory->length++;
                return FSMState::InvalidPiecePU;
            } else {
                Serial.println("FriendlyPU");
                Serial.println(instruction);
                Serial.println(current_square_moves->length);
                for (int i = 0; i < current_square_moves->length; i++) {
                    /////////////////////////////////
                    // this is for not allowing castling without the apropriate
                    // states this is dumb but I dont care
                    int curr = current_square_moves->moves[i].target_square;
                    int pieceId = current_square_moves->moves[i].piece;
                    if (pieceId == 43 && (curr == 118 || curr == 114) ||
                        pieceId == 51 && (curr == 6 || curr == 2)) {
                        state_memory->memory[1] = instruction;
                        state_memory->length++;
                        return FSMState::InvalidMove;
                    }
                    /////////////////////////////////
                    if (current_square_moves->moves[i].target_square ==
                        instruction) {
                        if (current_square_moves->moves[i].capture != 0) {
                            state_memory->memory[1] = instruction;
                            state_memory->length++;
                            return FSMState::FriendlyAndEnemyPU;
                        }
                        chess->make_move(current_square_moves->moves[i]);
                        // std::cout << "MOVE COMMITTED" << std::endl;
                        Serial.println("MOVE COMMITTED");
                        return FSMState::MoveComplete;
                    }
                }
                state_memory->memory[1] = instruction;
                state_memory->length++;
                return FSMState::InvalidMove;
            }
            break;
        case EnemyPU:
            if (instruction == state_memory->memory[0]) {
                state_memory->length--;
                return FSMState::Idle;
            } else if ((enemies & square) != 0ULL) {
                state_memory->memory[1] = instruction;
                state_memory->length++;
                return FSMState::InvalidPiecePU;
            } else {
                for (int i = 0; i < current_square_moves->length; i++) {
                    if (current_square_moves->moves[i].source_square ==
                        instruction) {
                        state_memory->memory[1] = instruction;
                        state_memory->length++;
                        return FSMState::FriendlyAndEnemyPU;
                    }
                }
            }
            break;
        case FriendlyAndEnemyPU:
            int prev_friendly_square;
            int prev_enemy_square;
            if (is_friendly(chess, state_memory->memory[0])) {
                prev_friendly_square = state_memory->memory[0];
                prev_enemy_square = state_memory->memory[1];
            } else {
                prev_friendly_square = state_memory->memory[1];
                prev_enemy_square = state_memory->memory[0];
            }

            if (instruction == prev_friendly_square) {
                state_memory->memory[0] = prev_enemy_square;
                state_memory->length--;
                return FSMState::EnemyPU;
            } else if (instruction == prev_enemy_square) {
                // std::cout << "CAPTURED!" << std::endl;
                for (int i = 0; i < current_square_moves->length; i++) {
                    if (current_square_moves->moves[i].target_square ==
                            prev_enemy_square &&
                        current_square_moves->moves[i].source_square ==
                            prev_friendly_square) {
                        chess->make_move(current_square_moves->moves[i]);
                        // std::cout << "MOVE COMMITTED" << std::endl;
                        return FSMState::MoveComplete;
                    }
                }
            } else {
                return FSMState::Error;
            }
            break;
        case Castling:
            if (instruction == state_memory->memory[1]) {
                state_memory->length--;
                return FSMState::FriendlyPU;
            } else if (chess->get_side() == WHITE) {
                if (instruction == 114) {
                    state_memory->memory[1] = instruction;
                    state_memory->memory[2] = 115;
                    state_memory->length += 2;
                    // chess->make_move(chess->parse_move("e1c1"));
                    return FSMState::CastlingPutRookDown;
                } else if (instruction == 118) {
                    state_memory->memory[1] = instruction;
                    state_memory->memory[2] = 117;
                    state_memory->length += 2;
                    // chess->make_move(chess->parse_move("e1g1"));
                    return FSMState::CastlingPutRookDown;
                } else {
                    return FSMState::Error;
                }
            } else if (chess->get_side() == BLACK) {
                if (instruction == 2) {
                    state_memory->memory[1] = instruction;
                    state_memory->memory[2] = 3;
                    state_memory->length++;
                    // chess->make_move(chess->parse_move("e8c8"));
                    return FSMState::CastlingPutRookDown;
                } else if (instruction == 6) {
                    state_memory->memory[1] = instruction;
                    state_memory->memory[2] = 5;
                    state_memory->length++;
                    // chess->make_move(chess->parse_move("e8g8"));
                    return FSMState::CastlingPutRookDown;
                } else {
                    return FSMState::Error;
                }
            } else {
                return FSMState::Error;
            }
            break;
        case CastlingPutRookDown:
            if (instruction == state_memory->memory[1]) {
                state_memory->length--;
                return FSMState::Castling;
            } else if (instruction == state_memory->memory[2]) {
                for (int i = 0; i < current_square_moves->length; i++) {
                    if (current_square_moves->moves[i].target_square ==
                        state_memory->memory[1]) {
                        // std::cout << current_square_moves->moves[i] <<
                        // std::endl;
                        chess->make_move(current_square_moves->moves[i]);
                        // std::cout << "MOVE COMMITTED" << std::endl;
                        return FSMState::MoveComplete;
                    }
                }
                return FSMState::Error;
            } else {
                return FSMState::Error;
            }
            break;
        case InvalidPiecePU:
            if (state_memory->length == 1 &&
                state_memory->memory[0] == instruction) {
                state_memory->length--;
                return FSMState::Idle;
            } else if (state_memory->length == 2 &&
                       is_friendly(chess, instruction)) {
                state_memory->length--;
                return FSMState::FriendlyPU;
            } else if (state_memory->length == 2 &&
                       !is_friendly(chess, instruction)) {
                state_memory->length--;
                return FSMState::EnemyPU;
            } else {
                return FSMState::Error;
            }
            break;
        case InvalidMove:
            if (instruction == state_memory->memory[1]) {
                state_memory->length--;
                return FSMState::FriendlyPU;
            } else {
                return FSMState::Error;
            }
    }

    return state;
}

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                                      Main ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

// int main() {
//     // variable setup
//     Chess chess;
//     enum FSMState state = Idle;
//     Move_List move_list[1];
//     Move_List current_square_moves[1];
//     State_Memory state_memory[1];

//     printf("hello world\n");
//     printf("%d \n", state);

//     print_bitboard(chess.get_occupied());
//     printf("\n");
//     int side = 8;  // 8 = white, 16 = black
//     while (true) {
//         chess.generate_moves(side, 128 /*change later*/, move_list, 0);
//         chess.print_board();
//         while (true) {
//             int square;
//             printf("Type number of square: \n");
//             scanf("%d", &square);
//             printf("the square is %d \n", square);
//             state = update_state(&chess, square, side, state, state_memory,
//                                  move_list, current_square_moves);
//             printf("current state is: %d \n", state);
//             printf("FSMState memory 0: %d\n", state_memory->memory[0]);
//             printf("FSMState memory 1: %d\n", state_memory->memory[1]);
//             if (state == FSMState::MoveComplete) break;
//         }
//         state = FSMState::Idle;
//         printf("move complete\n");
//         state_memory->length = 0;
//         side = 24 - side;
//     }
// }

std::string fsm_state_string(FSMState state) {
    switch (state) {
        case FSMState::Idle:
            return "Idle";
        case FSMState::FriendlyPU:
            return "FriendlyPU";
        case FSMState::EnemyPU:
            return "EnemyPU";
        case FSMState::FriendlyAndEnemyPU:
            return "FriendlyAndEnemyPU";
        case FSMState::Castling:
            return "Castling";
        case FSMState::CastlingPutRookDown:
            return "CastlingPutRookDown";
        case FSMState::InvalidPiecePU:
            return "InvalidPiecePU";
        case FSMState::InvalidMove:
            return "InvalidMove";
        case FSMState::Error:
            return "Error";
        case FSMState::MoveComplete:
            return "MoveComplete";
        default:
            return "Unknown";
    }
}
