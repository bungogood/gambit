#pragma once

#include <vector>

#include "chess.hpp"

// 19 horizontal 17 vertical
typedef struct BPoint {
    int x;
    int y;
    BPoint(int x, int y) : x(x), y(y) {}
} BPoint;

const int BLACK_CAPTURE = 0;
const int BLACK_SLIDE = 1;
const int WHITE_CAPTURE = 18;
const int WHITE_SLIDE = 17;

class MoveStep {
   public:
    BPoint point;
    bool magnet;

    MoveStep(BPoint point, bool magnet);
    MoveStep(Square square, bool magnet);
};

std::vector<MoveStep> castlingSteps(Move move);
std::vector<MoveStep> knightShuffle(Move move);
std::vector<MoveStep> captureSteps(Move move);
std::vector<MoveStep> moveSteps(Move move);
