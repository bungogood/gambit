#include "chess.hpp"

// clang-format off
/*********************************************************************************\
;                               nibble-chess v1.0                                 ;
;---------------------------------------------------------------------------------;
;                    A tribute to chess programming community                     ;
;              based on the ideas taken from micro-Max by H.G.Muller              ;
;---------------------------------------------------------------------------------;
;                                by Maksim Korzh                                  ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;           THIS WORK IS DEDICATED TO HOBBY PROGRAMMERS WHO AIMS TO GET           ;
;                        THE VERY GIST OF CHESS PROGRAMMING                       ;
;---------------------------------------------------------------------------------;
;       "A vague understanding of your goals leads to unpredictable results       ;
;             and abandoning the project halfway..." - my experience              ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;        Copyright © 2018 Maksim Korzh <freesoft.for.people@gmail.com>            ;
;---------------------------------------------------------------------------------;
;     This work is free. You can redistribute it and/or modify it under the       ;
;      terms of the Do What The Fuck You Want To Public License, Version 2,       ;
;       as published by Sam Hocevar. See the COPYING file for more details.       ;
;---------------------------------------------------------------------------------;
'       THIS PROGRAM IS FREE SOFTWARE. IT COMES WITHOUT ANY WARRANTY, TO          ;
;        THE EXTENT PERMITTED BY APPLICABLE LAW. YOU CAN REDISTRIBUTE IT          ;
;       AND/OR MODIFY IT UNDER THE TERMS OF THE DO WHAT THE FUCK YOU WANT         ;
;          TO PUBLIC LICENCE, VERSION 2, AS PUBLISHED BY SAM HOCEVAR.             ;
;                SEE http://www.wtfpl.net/ FOR MORE DETAILS.                      ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                   DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE                   ;
;                           Version 2, December 2004                              ;
;                                                                                 ;
;        Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>                         ;
;                                                                                 ;
;        Everyone is permitted to copy and distribute verbatim or modified        ;
;        copies of this license document, and changing it is allowed as long      ;
;        as the name is changed.                                                  ;
;                                                                                 ;
;                   DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE                   ;
;          TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION        ;
;                                                                                 ;
;         0. You just DO WHAT THE FUCK YOU WANT TO.                               ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//                                  Variables                                    //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//    S - side                                                                   //
//    E - en passant square                                                      //
//    V - move structure                                                         //
//    L - move list structure                                                    //
//    Q - search info structure                                                  //
//                                                                               //
//    b - board array                                                            //
//                                                                               //
//    d - move direction                                                         //
//  v.f - source square                                                          //
//  v.t - target square                                                          //
//  v.p - piece                                                                  //
//  v.y - piece type                                                             //
//  v.r - step vector ray                                                        //
//  v.x - capture                                                                //
//  v.c - captured square                                                        //
//  v.K - skip square                                                            //
//  v.R - rook square                                                            //
//  v.o - promoted piece                                                         //
//  v.e - evaluation score                                                       //
//  v.s - move score                                                             //
//                                                                               //
//  q.n - nodes                                                                  //
//  q.m - best move                                                              //
//  q.s - best score                                                             //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//                                Piece encoding                                 //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  emSq, P+, P-, K, N, B, R, Q                                                  //
//     0,  1,  2, 3, 4, 5, 6, 7                                                  //
//                                                                               //
//  w = 8  b = 16  virgin = 32                                                   //
//                                                                               //
//  wP : P+ | w = 9                                                              //
//  wK :  K | w = 11    wKv : wK | virgin = 43                                   //
//  wN :  N | w = 12                                                             //
//  wB :  B | w = 13                                                             //
//  wR :  R | w = 14    wRv : wR | virgin = 46                                   //
//  wQ :  Q | w = 15                                                             //
//                                                                               //
//  bP : P- | b = 18                                                             //
//  bK :  K | b = 19    bKv : bK | virgin = 51                                   //
//  bN :  N | b = 20                                                             //
//  bB :  B | b = 21                                                             //
//  bR :  R | b = 22    bRv : bR | virgin = 54                                   //
//  bQ :  Q | b = 23                                                             //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//                                   Functions                                   //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  M(S, V m)/U(S, V m) - make/unmake move                                       //
//                                                                               //
//        S - side                                                               //
//      V m - move                                                               //
//                                                                               //
//  G(S, E, l, x) - generate moves                                               //
//                                                                               //
//      S - side                                                                 //
//      E - e.p.                                                                 //
//      l - move list                                                            //
//      x - all moves/only captures flag                                         //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  B(S) - evaluate position                                                     //
//                                                                               //                     
//      S - side                                                                 //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  X(S, E, a, k, d, Q *q) - search position                                     //
//                                                                               //
//      S - side                                                                 //
//      E - e.p.                                                                 //
//      a - alpha                                                                //
//      k - beta                                                                 //
//      d - depth                                                                //
//   Q *q - pointer to search info                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  R(S, E, a, k) - quiescence search                                            //
//                                                                               //
//      S - side                                                                 //
//      E - e.p.                                                                 //
//      a - alpha                                                                //
//      k - beta                                                                 //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  Y(S, E, *m) - parse move                                                     //
//                                                                               //
//      S - side                                                                 //
//      E - en passant square                                                    //
//     *m - move string e.g. "e2e4"                                              //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  P() - print board                                                            //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
// clang-format on
#include <stdio.h>
#include <string.h>

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                              BOARD REPRESENTATION ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

// promoted pieces
const char promoted_pieces[8] = {0, 0, 0, 0, 'n', 'b', 'r', 'q'};

const char promoted_pieces_string[] = ".-pknbrq-P-KNBRQ";

// move offsets
static const int move_offsets[] = {
    15, 16,  17, 0,   -15, -16, -17, 0,   1,  16, -1,  -16, 0,
    1,  16,  -1, -16, 15,  -15, 17,  -17, 0,  14, -14, 18,  -18,
    31, -31, 33, -33, 0,   3,   -1,  12,  21, 16, 7,   12};

// piece weights
static const int piece_weights[] = {0, 0,   -100, 0, -300, -350, -500, -900,
                                    0, 100, 0,    0, 300,  350,  500,  900};

Chess::Chess() : side(WHITE), en_passant(128) {
    // initialize board
}

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                                Helper Functions ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

uint64_t Chess::get_occupied() {
    uint64_t output = 0;
    int offset = 0;  // to skip over the unwanted parts of board_array
    for (int i = 0; i < 64; i++) {
        uint64_t temp = 0;
        if (i != 0 && i % 8 == 0) {
            offset += 8;
        }
        if (board_array[i + offset] > 0) {
            temp = 1ULL << (63 - i);
            output += temp;
        }
    }
    return output;
}

uint64_t Chess::get_white() {
    uint64_t output = 0;
    int offset = 0;
    for (int i = 0; i < 64; i++) {
        uint64_t temp = 0;
        if (i != 0 && i % 8 == 0) {
            offset += 8;
        }
        if (board_array[i + offset] > 0 && board_array[i + offset] < 18 ||
            board_array[i + offset] == 43 || board_array[i + offset] == 46) {
            temp = 1ULL << (63 - i);
            output += temp;
        }
    }
    return output;
}

uint64_t Chess::get_black() {
    uint64_t output = 0;
    int offset = 0;
    for (int i = 0; i < 64; i++) {
        uint64_t temp = 0;
        if (i != 0 && i % 8 == 0) {
            offset += 8;
        }
        if (board_array[i + offset] >= 18 && board_array[i + offset] < 24 ||
            board_array[i + offset] == 51 || board_array[i + offset] == 54) {
            temp = 1ULL << (63 - i);
            output += temp;
        }
    }
    return output;
}

char Chess::square_char(int square) {
    // do in 0-63
    int index = square / 8 * 16 + square % 8;
    int piece = board_array[index];
    return promoted_pieces_string[piece & 15];
}

int Chess::get_piece_on_square(int square) { return board_array[square]; }

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                                MOVE GENERATION ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

inline void Chess::make_move(Move move) {
    board_array[move.rook_square] = board_array[move.captured_square] =
        board_array[move.source_square] = 0;
    board_array[move.target_square] = move.piece & 31;
    en_passant = move.skip_square;

    if (!(move.rook_square & 0x88)) board_array[move.skip_square] = side + 6;

    if (move.piece_type < 3) {
        if (move.target_square + move.step_vector_ray + 1 & 128)
            board_array[move.target_square] = side + move.promoted_piece;
    }

    side = 24 - side;
}

inline void Chess::unmake_move(Move move, Square old_ep) {
    side = 24 - side;
    en_passant = old_ep;
    board_array[move.rook_square] = side + 38;
    board_array[move.skip_square] = board_array[move.target_square] = 0;
    board_array[move.source_square] = move.piece;
    board_array[move.captured_square] = move.capture;
}

inline int Chess::evaluate_position(Player side) {
    int score = 0;
    int i = 0, position;

    do {
        position = board_array[i];
        if (position) {
            score += piece_weights[position & 15];  // material score
            (position & 8) ? (score += board_array[i + 8])
                           : (score -= board_array[i + 8]);  // positional score
        }
        i = (i + 9) & ~0x88;
    } while (i);

    return (side == 8) ? score : -score;
}

int Chess::generate_moves(Move_List *move_list, bool only_captures) {
    Move move;
    move.promoted_piece = 0;
    int directions;
    move_list->length = 0;
    move.source_square = 0;
    int old_ep = en_passant;

    do {  // loop over board pieces
        move.piece = board_array[move.source_square];

        if (move.piece & side) {
            move.piece_type = move.piece & 7;
            directions = move_offsets[move.piece_type + 30];
            move.step_vector_ray = move_offsets[++directions];
            while (move.step_vector_ray) {  // loop over directions
                move.target_square = move.source_square;
                move.skip_square = move.rook_square = 128;

                do {  // loop over squares
                    move.target_square += move.step_vector_ray;
                    move.captured_square = move.target_square;

                    if (move.target_square & 0x88) break;
                    if (move.piece_type < 3 && move.target_square == en_passant)
                        move.captured_square = move.target_square ^ 16;
                    move.capture = board_array[move.captured_square];
                    if (en_passant - 128 && board_array[en_passant] &&
                        move.target_square - en_passant < 2 &&
                        en_passant - move.target_square < 2)
                        return 0;
                    if (move.capture & side ||
                        move.piece_type < 3 &&
                            !(move.step_vector_ray & 7) != !move.capture)
                        break;
                    if ((move.capture & 7) == 3) return move_list->length = 0;

                    make_move(move);  // make move

                    if (move.piece_type < 3) {
                        if (move.target_square + move.step_vector_ray + 1 &
                            128) {
                            board_array[move.target_square] |= 7;
                            move.promoted_piece =
                                board_array[move.target_square] & 7;
                        };
                    }

                    do {
                        move.move_score = evaluate_position(
                            side);  // evaluate position for move ordering
                        if (only_captures && move.capture) {
                            move_list->moves[move_list->length] = move;
                            move_list->length++;
                        } else if (!only_captures) {
                            move_list->moves[move_list->length] = move;
                            move_list->length++;
                        }

                        (move.promoted_piece < 4) ? move.promoted_piece = 0
                                                  : move.promoted_piece--;
                    }

                    while (move.piece_type - board_array[move.target_square]-- &
                               7 &&
                           board_array[move.target_square] & 4);

                    unmake_move(move, old_ep);  // take back

                    move.capture += move.piece_type < 5;

                    if (move.piece_type < 3 &&
                            6 * side + (move.target_square & 112) == 128 ||
                        (((move.piece & ~24) == 35) &
                         (directions == 13 || directions == 15)) &&
                            move.rook_square & 0x88 &&
                            board_array[move.rook_square =
                                            (move.source_square | 7) -
                                            (move.step_vector_ray >> 1 & 7)] &
                                32 &&
                            !(board_array[move.rook_square ^ 1] |
                              board_array[move.rook_square ^ 2])) {
                        move.capture--;
                        move.skip_square = move.target_square;
                    }
                }

                while (!move.capture);
                move.step_vector_ray = move_offsets[++directions];
            }
        }
        move.source_square = (move.source_square + 9) & ~0x88;
    } while (move.source_square);
    return 1;
}

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                                      SEARCH ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

int Chess::quiescence_search(Player side, int alpha,
                             int beta) {  // QUIESCENCE SEARCH
    int score = evaluate_position(side);
    Square old_ep = en_passant;

    if (score >= beta) return beta;
    if (score > alpha) alpha = score;

    Move_List move_list[1];

    if (!generate_moves(move_list, true)) return 10000;  // checkmate evaluation

    for (int i = 0; i < move_list->length; i++) {  // loop over move list
        for (int j = i + 1; j < move_list->length; j++) {
            // order moves to reduce number of traversed nodes
            if (move_list->moves[i].move_score <
                move_list->moves[j].move_score) {
                Move move = move_list->moves[i];
                move_list->moves[i] = move_list->moves[j];
                move_list->moves[j] = move;
            }
        }

        make_move(move_list->moves[i]);  // make move
        int score = -quiescence_search(24 - side, -beta, -alpha);
        unmake_move(move_list->moves[i], old_ep);  // take back

        if (score >= beta) return beta;
        if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}

int Chess::search_position(Player side, int alpha, int beta, int depth,
                           Search_Info *search_info, Move_List *move_lists) {
    // Move_List move_list[1];
    Move_List *move_list = &move_lists[depth - 1];
    int old_alpha = alpha;
    Square old_ep = en_passant;
    Move move;

    // if (!depth) return quiescence_search(side, alpha, beta);
    if (!depth) return evaluate_position(side);
    if (!generate_moves(move_list, false))
        return 10000;  // checkmate evaluation

    for (int i = 0; i < move_list->length; i++) {  // loop over move list
        for (int j = i + 1; j < move_list->length; j++) {
            // order moves to reduce number of traversed nodes
            if (move_list->moves[i].move_score <
                move_list->moves[j].move_score) {
                Move temp_move = move_list->moves[i];
                move_list->moves[i] = move_list->moves[j];
                move_list->moves[j] = temp_move;
            }
        }

        make_move(move_list->moves[i]);  // make move
        int score = -search_position(24 - side, -beta, -alpha, depth - 1,
                                     search_info, move_lists);
        unmake_move(move_list->moves[i], old_ep);  // take back

        search_info->best_move = move_list->moves[i];  // store best move so far
        search_info->best_score = score;

        if (score >= beta) return beta;
        if (score > alpha) {
            alpha = score;
            move = move_list->moves[i];
        }
    }

    if (alpha != old_alpha) {
        search_info->best_move = move;  // store best move
        search_info->best_score = alpha;
    }

    return alpha;
}

void Chess::get_best_move(int depth, Search_Info *search_info,
                          Move_List *move_lists) {
    search_position(side, -10000, 10000, depth, search_info, move_lists);
}

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                                       MAIN ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

Move Chess::parse_move(const char *move_string, Move_List *move_list) {
    Move move;
    generate_moves(move_list, false);

    for (int i = 0; i < move_list->length; i++) {
        move = move_list->moves[i];

        if (move.source_square == (move_string[0] - 'a') +
                                      (7 - (move_string[1] - '0' - 1)) * 16 &&
            move.target_square == (move_string[2] - 'a') +
                                      (7 - (move_string[3] - '0' - 1)) * 16) {
            if (move.promoted_piece) {
                if (promoted_pieces[move.promoted_piece] == move_string[4])
                    return move;
                continue;
            }

            return move;
        }
    }

    move.promoted_piece = move.target_square = move.source_square = 0;
    return move;
}

void Chess::print_board() {
    for (int i = 0; i < 128; i++) {
        if (!(i % 16)) printf(" %d  ", 8 - (i / 16));
        printf(" %c", ((i & 8) && (i += 7))
                          ? '\n'
                          : promoted_pieces_string[board_array[i] & 15]);
    }

    printf("\n     a b c d e f g h\n\nYour move: ");
}

// TODO: Castling rights, en passant, etc.
std::string Chess::get_fen() {
    std::string fen = "";
    int empty_count = 0;

    for (int i = 0; i < 64; i++) {
        char piece = square_char(i);
        if (piece == '.') {
            empty_count++;
        } else {
            if (empty_count) {
                fen += empty_count + '0';
                empty_count = 0;
            }
            fen += piece;
        }

        if ((i + 1) % 8 == 0) {
            if (empty_count) {
                fen += empty_count + '0';
                empty_count = 0;
            }

            if (i != 63) fen += '/';
        }
    }

    // side to move
    fen += side == WHITE ? " w" : " b";

    // castling rights
    fen += " KQkq";

    // en passant
    fen += en_passant == 128 ? " -" : " " + square_to_string(en_passant);

    // halfmove clock
    fen += " 0";

    // fullmove number
    fen += " 1";

    return fen;
}

std::string square_to_string(const Square square) {
    std::string str = "";
    str += 'a' + (square & 7);
    str += '1' + 7 - (square >> 4);
    return str;
}

std::string move_to_string(const Move move) {
    std::string str = "";
    str += square_to_string(move.source_square);
    str += square_to_string(move.target_square);
    return str;
}
