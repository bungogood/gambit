#pragma once

typedef int Square;

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
        54, 20, 21, 23, 51, 21, 20, 54,    0,  0,  5,  0, -5,  0,  5,  0, 
        18, 18, 18, 18, 18, 18, 18, 18,    5,  5,  0,  0,  0,  0,  5,  5,
        0,  0,  0,  0,  0,  0,  0,  0,    5, 10, 15, 20, 20, 15, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0,    5, 10, 20, 30, 30, 20, 10,  5,    
        0,  0,  0,  0,  0,  0,  0,  0,    5, 10, 20, 30, 30, 20, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0,    5, 10, 15, 20, 20, 15, 10,  5,
        9,  9,  9,  9,  9,  9,  9,  9,    5,  5,  0,  0,  0,  0,  5,  5,
        46, 12, 13, 15, 43, 13, 12, 46,    0,  0,  5,  0, -5,  0,  5,  0
    };
public:
    Chess();
    void print_board();
    Move parse_move(int side, int en_passant, char *move_string);
    int search_position(int side, int en_passant, int alpha, int beta, int depth, Search_Info *search_info);
    int quiescence_search(int side, int en_passant, int alpha, int beta);
    inline int generate_moves(int side, int en_passant, Move_List *move_list, bool only_captures);
    inline int evaluate_position(int side);
    inline void unmake_move(int side, Move move);
    void make_move(int side, Move move);
};
