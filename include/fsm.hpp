#pragma once

#include "chess.hpp"

enum FSMState : int {
    Idle,
    FriendlyPU,
    EnemyPU,
    FriendlyAndEnemyPU,
    Castling,
    CastlingPutRookDown,
    InvalidPiecePU,
    InvalidMove,
    Error,
    MoveComplete
};

std::string fsm_state_string(FSMState state);

typedef struct {
    int memory[3];
    int length;
} State_Memory;

FSMState update_state(Chess *chess, int instruction, FSMState state,
                      State_Memory *state_memory, Move_List *move_list,
                      Move_List *current_square_moves);

void print_bitboard(unsigned long long bitboard);