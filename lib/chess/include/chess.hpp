#pragma once

#include <string>
#include <cstdint>
typedef int Square;
typedef int Player;

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

std::string square_to_string(const Square square);
std::string move_to_string(const Move move);

extern Move_List chess_move_lists[3];

class Chess {
   private:
    int board_array[129] = {  // 0x88 board + centers positional scores
        54, 20, 21, 23, 51, 21, 20, 54, 0, 0,  5,  0,  -5, 0,  5,  0,
        18, 18, 18, 18, 18, 18, 18, 18, 5, 5,  0,  0,  0,  0,  5,  5,
        0,  0,  0,  0,  0,  0,  0,  0,  5, 10, 15, 20, 20, 15, 10, 5,
        0,  0,  0,  0,  0,  0,  0,  0,  5, 10, 20, 30, 30, 20, 10, 5,
        0,  0,  0,  0,  0,  0,  0,  0,  5, 10, 20, 30, 30, 20, 10, 5,
        0,  0,  0,  0,  0,  0,  0,  0,  5, 10, 15, 20, 20, 15, 10, 5,
        9,  9,  9,  9,  9,  9,  9,  9,  5, 5,  0,  0,  0,  0,  5,  5,
        46, 12, 13, 15, 43, 13, 12, 46, 0, 0,  5,  0,  -5, 0,  5,  0};
    Square en_passant;
    Player side;

   public:
    Chess();
    uint64_t get_occupied();
    uint64_t get_white();
    uint64_t get_black();
    int get_piece_on_square(int square);
    void print_board();
    Move parse_move(const char *move_string, Move_List *move_list);
    int search_position(Player side, int alpha, int beta, int depth,
                        Search_Info *search_info, Move_List *move_lists);
    int quiescence_search(Player side, int alpha, int beta);
    int generate_moves(Move_List *move_list, bool only_captures = false);
    inline int evaluate_position(Player side);
    void unmake_move(Move move, Square old_ep);
    void make_move(Move move);

    void get_best_move(int depth, Search_Info *search_info,
                       Move_List *move_lists);

    char square_char(int square);
    std::string get_fen();
    // void set_fen(std::string fen);

    Player get_side() { return side; }
    Square get_en_passant() { return en_passant; }
};
