#pragma once

#include "chess.hpp"

enum State {
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

typedef struct {
    int memory[3];
    int length;
} State_Memory;