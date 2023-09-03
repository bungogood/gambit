#pragma once

typedef int Square;

const int WHITE = 8;
const int BLACK = 16;

const int WPAWN = 1;
const int BPAWN = 2;
const int KING = 33;
const int KNIGHT = 4;
const int BISHOP = 5;
const int ROOK = 6;
const int QUEEN = 7;

// clang-format off
const Square 
    SQA8 =  0, SQB8 =  1, SQC8 =  2, SQD8 =  3, SQE8 =  4, SQF8 =  5, SQG8 =  6, SQH8 =  7,
    SQA7 = 16, SQB7 = 17, SQC7 = 18, SQD7 = 19, SQE7 = 20, SQF7 = 21, SQG7 = 22, SQH7 = 23,
    SQA6 = 32, SQB6 = 33, SQC6 = 34, SQD6 = 35, SQE6 = 36, SQF6 = 37, SQG6 = 38, SQH6 = 39,
    SQA5 = 48, SQB5 = 49, SQC5 = 50, SQD5 = 51, SQE5 = 52, SQF5 = 53, SQG5 = 54, SQH5 = 55,
    SQA4 = 64, SQB4 = 65, SQC4 = 66, SQD4 = 67, SQE4 = 68, SQF4 = 69, SQG4 = 70, SQH4 = 71,
    SQA3 = 80, SQB3 = 81, SQC3 = 82, SQD3 = 83, SQE3 = 84, SQF3 = 85, SQG3 = 86, SQH3 = 87,
    SQA2 = 96, SQB2 = 97, SQC2 = 98, SQD2 = 99, SQE2 =100, SQF2 =101, SQG2 =102, SQH2 =103,
    SQA1 =112, SQB1 =113, SQC1 =114, SQD1 =115, SQE1 =116, SQF1 =117, SQG1 =118, SQH1 =119;
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
    Move parse_move(const char *move_string, int side, int en_passant = 128);
    int search_position(int side, int en_passant, int alpha, int beta,
                        int depth, Search_Info *search_info);
    int quiescence_search(int side, int en_passant, int alpha, int beta);
    int generate_moves(int side, int en_passant, Move_List *move_list,
                       bool only_captures);
    inline int evaluate_position(int side);
    void unmake_move(Move move, int side);
    void make_move(Move move, int side);
};
