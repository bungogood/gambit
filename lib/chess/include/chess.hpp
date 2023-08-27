#pragma once

typedef int Square;

const int KNIGHT = 4;

// clang-format off
const Square 
    SQA1 =  0, SQB1 =  1, SQC1 =  2, SQD1 =  3, SQE1 =  4, SQF1 =  5, SQG1 =  6, SQH1 =  7,
    SQA2 =  8, SQB2 =  9, SQC2 = 10, SQD2 = 11, SQE2 = 12, SQF2 = 13, SQG2 = 14, SQH2 = 15,
    SQA3 = 16, SQB3 = 17, SQC3 = 18, SQD3 = 19, SQE3 = 20, SQF3 = 21, SQG3 = 22, SQH3 = 23,
    SQA4 = 24, SQB4 = 25, SQC4 = 26, SQD4 = 27, SQE4 = 28, SQF4 = 29, SQG4 = 30, SQH4 = 31,
    SQA5 = 32, SQB5 = 33, SQC5 = 34, SQD5 = 35, SQE5 = 36, SQF5 = 37, SQG5 = 38, SQH5 = 39,
    SQA6 = 40, SQB6 = 41, SQC6 = 42, SQD6 = 43, SQE6 = 44, SQF6 = 45, SQG6 = 46, SQH6 = 47,
    SQA7 = 48, SQB7 = 49, SQC7 = 50, SQD7 = 51, SQE7 = 52, SQF7 = 53, SQG7 = 54, SQH7 = 55,
    SQA8 = 56, SQB8 = 57, SQC8 = 58, SQD8 = 59, SQE8 = 60, SQF8 = 61, SQG8 = 62, SQH8 = 63;
// clang-format on

typedef struct {
    Square source_square;
    Square target_square;
    int piece;
    int piece_type;
    int capture;
    Square captured_square;
    int step_vector_ray;
    Square rook_square;
    Square skip_square;
    int promoted_piece;
    int evaluation_score;
    int move_score;
} Move;

typedef struct {
    Move moves[256];
    int length;
} Move_List;

typedef struct {
    int best_score;
    Move best_move;
} Search_Info;

class Chess {
    int board_array[129] = {  // 0x88 board + centers positional scores
        54, 20, 21, 23, 51, 21, 20, 54, 0, 0,  5,  0,  -5, 0,  5,  0,
        18, 18, 18, 18, 18, 18, 18, 18, 5, 5,  0,  0,  0,  0,  5,  5,
        0,  0,  0,  0,  0,  0,  0,  0,  5, 10, 15, 20, 20, 15, 10, 5,
        0,  0,  0,  0,  0,  0,  0,  0,  5, 10, 20, 30, 30, 20, 10, 5,
        0,  0,  0,  0,  0,  0,  0,  0,  5, 10, 20, 30, 30, 20, 10, 5,
        0,  0,  0,  0,  0,  0,  0,  0,  5, 10, 15, 20, 20, 15, 10, 5,
        9,  9,  9,  9,  9,  9,  9,  9,  5, 5,  0,  0,  0,  0,  5,  5,
        46, 12, 13, 15, 43, 13, 12, 46, 0, 0,  5,  0,  -5, 0,  5,  0};

   public:
    Chess();
    unsigned long long get_occupied();
    unsigned long long get_white();
    unsigned long long get_black();
    int get_piece_on_square(int square);
    void print_board();
    Move parse_move(int side, int en_passant, char *move_string);
    int search_position(int side, int en_passant, int alpha, int beta,
                        int depth, Search_Info *search_info);
    int quiescence_search(int side, int en_passant, int alpha, int beta);
    int generate_moves(int side, int en_passant, Move_List *move_list,
                       bool only_captures);
    inline int evaluate_position(int side);
    inline void unmake_move(int side, Move move);
    void make_move(int side, Move move);
};
