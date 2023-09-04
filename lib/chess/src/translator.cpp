#include "translator.hpp"

#include <cmath>

MoveStep::MoveStep(BPoint point, bool magnet) : point(point), magnet(magnet) {}
MoveStep::MoveStep(Square square, bool magnet)
    : point(BPoint((1 + square % 16) * 2, (8 - square / 16) * 2)),
      magnet(magnet) {}

/**
 * @brief Castling steps
 *
 * @param move Castling move
 * @return Steps to castle
 */
std::vector<MoveStep> castlingSteps(Move move) {
    std::vector<MoveStep> steps;
    int sx = move.source_square % 16;
    int sy = 7 - move.source_square / 16;
    int tx = move.target_square % 16;
    int ty = 7 - move.target_square / 16;

    int rx = move.rook_square % 16;
    int ry = 7 - move.rook_square / 16;
}

/**
 * @brief Knight shuffle to half square
 *
 * @param move Knight move
 * @return Steps to half square
 */
std::vector<MoveStep> knightShuffle(Move move) {
    int sx = move.source_square % 16;
    int sy = 7 - move.source_square / 16;
    int tx = move.target_square % 16;
    int ty = 7 - move.target_square / 16;
    std::vector<MoveStep> steps;

    if (abs(tx - sx) > abs(ty - sy)) {
        int halfSquare = (sy + 1) * 2 + ty - sy;
        // move along y
        steps.push_back(MoveStep(BPoint((sx + 1) * 2, halfSquare), true));
        // move along x
        steps.push_back(MoveStep(BPoint((tx + 1) * 2, halfSquare), true));
    } else {
        int halfSquare = (sx + 1) * 2 + tx - sx;
        // move along x
        steps.push_back(MoveStep(BPoint(halfSquare, (sy + 1) * 2), true));
        // move along y
        steps.push_back(MoveStep(BPoint(halfSquare, (ty + 1) * 2), true));
    }
    return steps;
}

/**
 * @brief Capture steps
 *
 * @param move Capture move
 * @return Steps to capture
 */
std::vector<MoveStep> captureSteps(Move move) {
    std::vector<MoveStep> steps;
    int cx = move.captured_square % 16;
    int cy = 7 - move.captured_square / 16;

    if (move.capture) {
        steps.push_back(MoveStep(move.captured_square, false));
        if (move.capture & 8) {
            // capturing white
            int halfSquare = (cy + 1) * 2 + 1;
            steps.push_back(MoveStep(BPoint(cx, halfSquare), true));
            steps.push_back(MoveStep(BPoint(WHITE_SLIDE, halfSquare), true));
            steps.push_back(MoveStep(BPoint(WHITE_SLIDE, WHITE_CAPTURE), true));
            steps.push_back(
                MoveStep(BPoint(WHITE_CAPTURE, WHITE_CAPTURE), true));
        } else {
            // capturing black
            int halfSquare = (cy + 1) * 2 - 1;
            steps.push_back(MoveStep(BPoint(cx, halfSquare), true));
            steps.push_back(MoveStep(BPoint(BLACK_SLIDE, halfSquare), true));
            steps.push_back(MoveStep(BPoint(BLACK_SLIDE, BLACK_CAPTURE), true));
            steps.push_back(
                MoveStep(BPoint(BLACK_CAPTURE, BLACK_CAPTURE), true));
        }
    }
    return steps;
}

std::vector<MoveStep> moveSteps(Move move) {
    std::vector<MoveStep> steps;

    if (!(move.rook_square & 0x88)) return castlingSteps(move);

    if (move.capture) {  // capture
        std::vector<MoveStep> shuffle = captureSteps(move);
        steps.insert(steps.end(), shuffle.begin(), shuffle.end());
    }

    steps.push_back(MoveStep(move.source_square, false));
    if (move.piece_type == KNIGHT) {  // knight move
        std::vector<MoveStep> shuffle = knightShuffle(move);
        steps.insert(steps.end(), shuffle.begin(), shuffle.end());
    }
    steps.push_back(MoveStep(move.target_square, true));

    return steps;
}
