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
