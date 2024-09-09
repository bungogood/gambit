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
